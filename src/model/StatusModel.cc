#include "model/StatusModel.h"
#include "db/ConnectionPool.h"
#include "db/Connection.h"

void StatusModel::query(const string& username, vector<Status>& statuss)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from status where username = '%s'",username.c_str());

    // MySql mysql;
    // if (mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if (res != nullptr)
    //     {
    //         MYSQL_ROW row = nullptr;
    //         while ((row = mysql_fetch_row(res)) != nullptr)
    //         {
    //             Status status(
    //                 row[1],
    //                 atoi(row[2]),
    //                 row[3],
    //                 row[4],
    //                 row[5]
    //             );
    //             status.setSid(atoi(row[0]));
    //             statuss.push_back(status);
    //         }
    //     }
    // }
    shared_ptr<Connection> conn = ConnectionPool::getInstance()->getConnection();
    MYSQL_RES *res = conn->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row = nullptr;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            Status status(
                row[1],
                atoi(row[2]),
                row[3],
                row[4],
                row[5]
            );
            status.setSid(atoi(row[0]));
            statuss.push_back(status);
        }
    }
    mysql_free_result(res);
}

void StatusModel::insert(Status& status)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into status(username,pid,result,language,submittime) values('%s',%d,'%s','%s','%s')",
                status.getUsername().c_str(), status.getPid(), status.getResult().c_str(), status.getLanguage().c_str(), status.getSubmittime().c_str());

    // MySql mysql;
    // if(mysql.connect())
    // {
    //     if(mysql.update(sql))
    //     {
    //         status.setSid(mysql_insert_id(mysql.getConn()));
    //     }
    // }
    shared_ptr<Connection> conn = ConnectionPool::getInstance()->getConnection();
    if(conn->update(sql))
    {
        status.setSid(mysql_insert_id(conn->getConn()));
    }
}