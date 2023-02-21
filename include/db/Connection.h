#pragma once
#include <mysql/mysql.h>
#include <string>

using namespace std;

class Connection
{
public:
    Connection();
    ~Connection();
    bool connect(const string& ip, unsigned short port, const string& username, const string& password, const string& daname);
    bool update(const string& sql);
    MYSQL_RES* query(const string& sql);

    void refreshTime();         //设置入队时间
    clock_t getTime() const;    //获得空闲时间

    MYSQL* getConn() const;
private:
    MYSQL* conn_;
    clock_t time_;              //毫秒时间
};