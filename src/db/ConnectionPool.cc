#include "db/ConnectionPool.h"
#include "tinymuduo/base/Logging.h"
#include "db/Connection.h"
#include <thread>

ConnectionPool::ConnectionPool()
{
    if(loadConfigFile() == false)
    {
        LOG_FATAL("数据库配置文件打开失败");
        return;
    }

    for(int i = 0;i < initSize_;i++)
    {
        Connection* conn = new Connection();
        conn->connect(ip_, port_, username_, password_, dbname_);
        connQue_.push(conn);
        conn->refreshTime();
        connCnt_++;
    }

    thread produceConn(std::bind(&ConnectionPool::produceConnTask, this));
    produceConn.detach();

    thread scanConn(std::bind(&ConnectionPool::scanConnTask, this));
    scanConn.detach();
}

ConnectionPool* ConnectionPool::getInstance()
{
    static ConnectionPool connPool;
    return &connPool;
}

shared_ptr<Connection> ConnectionPool::getConnection()
{
    unique_lock<mutex> lock(mtx_);
    while(connQue_.empty())
    {
        if(cv_status::timeout == cv_.wait_for(lock,std::chrono::milliseconds(connectionTimeout_)))
        {
            if(connQue_.empty())
            {
                LOG_DEBUG("获取连接超时");
                return nullptr;
            }
        }
    }

    shared_ptr<Connection> sp(connQue_.front(),[&](Connection* conn){
        unique_lock<mutex> lock(mtx_);
        connQue_.push(conn);
        conn->refreshTime();
    });
    connQue_.pop();
    cv_.notify_all();
    return sp;
}

void ConnectionPool::produceConnTask()
{
    while(true)
    {
        unique_lock<mutex> lock(mtx_);
        while(!connQue_.empty())
        {
            cv_.wait(lock);
        }

        if(connCnt_ < maxSize_)
        {
            Connection* conn = new Connection();
            conn->connect(ip_, port_, username_, password_, dbname_);
            connQue_.push(conn);
            conn->refreshTime();
            connCnt_++;
        }
        cv_.notify_all();
    }
}

void ConnectionPool::scanConnTask()
{
    while(true)
    {
        this_thread::sleep_for(std::chrono::seconds(maxIdleTime_));

        unique_lock<mutex> lock(mtx_);
        while(connCnt_ > maxSize_)
        {
            Connection* conn = connQue_.front();
            if(conn->getTime() >= maxIdleTime_*1000)
            {
                connQue_.pop();
                delete conn;
                connCnt_--;
            }
            else
            {
                break;
            }
        }
    }
}

bool ConnectionPool::loadConfigFile()
{
    FILE* fp = fopen("../mysql.ini","r");
    if(fp == nullptr)
    {
        return false;
    }

    while (!feof(fp))
    {
        char buf[1024] = {0};
        fgets(buf, 1024, fp);
        string str = buf;
        int idx = str.find("=", 0);
        if (idx == -1)
        {
            continue;
        }
        int endidx = str.find("\n", idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endidx - idx - 1);
        if (key == "ip")
        {
            ip_ = value;
        }
        else if (key == "port")
        {
            port_ = atoi(value.c_str());
        }
        else if (key == "username")
        {
            username_ = value;
        }
        else if (key == "password")
        {
            password_ = value;
        }
        else if (key == "dbname")
        {
            dbname_ = value;
        }
        else if (key == "initSize")
        {
            initSize_ = atoi(value.c_str());
        }
        else if (key == "maxSize")
        {
            maxSize_ = atoi(value.c_str());
        }
        else if (key == "maxIdleTime")
        {
            maxIdleTime_ = atoi(value.c_str());
        }
        else if (key == "connectionTimeout")
        {
            connectionTimeout_ = atoi(value.c_str());
        }
    }

    return true;
}