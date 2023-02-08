#include "db/db.h"
#include "tinymuduo/base/Logging.h"

static string sever = "127.0.0.1";
static string user = "lrd";
static string password = "lrd520LRD";
static string dbname = "onlinejudge";

MySql::MySql()
{
    conn_ = mysql_init(nullptr);
}

MySql::~MySql()
{
    if(conn_ != nullptr)
        mysql_close(conn_);
}

bool MySql::connect()
{
    MYSQL* p = mysql_real_connect(conn_, sever.c_str(), user.c_str(),
                password.c_str(), dbname.c_str(), 3306, nullptr, 0);
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

bool MySql::update(string sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO("更新失败");
        LOG_INFO("mysql errno : %s", mysql_error(conn_));
        return false;
    }
    return true;
}

MYSQL_RES* MySql::query(string sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO("查询失败");
        LOG_INFO("mysql errno : %s", mysql_error(conn_));
    }
    return mysql_use_result(conn_);
}

MYSQL* MySql::getConn()
{
    return conn_;
}