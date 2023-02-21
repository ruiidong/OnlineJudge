#pragma once

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>

using namespace std;

class Connection;
class ConnectionPool
{
public:
    static ConnectionPool* getInstance();
    shared_ptr<Connection> getConnection();
private:
    ConnectionPool();

    bool loadConfigFile();
    void produceConnTask();
    void scanConnTask();

    string ip_;
    unsigned short port_;
    string username_;
    string password_;
    string dbname_;

    int initSize_;                  //初始连接数
    int maxSize_;                   //最大连接数
    int maxIdleTime_;               //最大空闲时间，单位为s
    int connectionTimeout_;         //获取连接超时时间，单位为ms

    queue<Connection*> connQue_;
    atomic_int connCnt_;

    mutex mtx_;
    condition_variable cv_;
};