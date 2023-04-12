#include "db/Connection.h"
#include "tinymuduo/base/Logging.h"

Connection::Connection()
{
    conn_ = mysql_init(nullptr);
}

Connection::~Connection()
{
    if(conn_ != nullptr)
    {
        mysql_close(conn_);
    }
}

bool Connection::connect(const string &ip, unsigned short port, const string &username, const string &password, const string &dbname)
{
    MYSQL* p = mysql_real_connect(conn_, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
    if(p != nullptr)
    {
        mysql_query(conn_, "set names UTF8");
        return true;
    }
    else
    {
        LOG_INFO("connect mysql fail");
        return false;
    }
}

bool Connection::update(const string &sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO("更新失败");
        LOG_INFO("mysql errno : %s", mysql_error(conn_));
        return false;
    }
    return true;
}

MYSQL_RES *Connection::query(const string &sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO("查询失败");
        cout << "---------------------------------------------------------------------------------" << endl;
        LOG_INFO("mysql errno : %s", mysql_error(conn_));
        cout << "---------------------------------------------------------------------------------" << endl;
    }
    return mysql_use_result(conn_);
}

void Connection::refreshTime()      // 设置入队时间
{
    time_ = clock();
}

clock_t Connection::getTime() const // 获得空闲时间
{
    return clock() - time_;
}

MYSQL* Connection::getConn() const
{
    return conn_;
}