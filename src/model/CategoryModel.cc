#include "model/CategoryModel.h"
#include "db/Connection.h"
#include "db/ConnectionPool.h"

// CREATE TABLE category(
// cid INT(11) PRIMARY KEY AUTO_INCREMENT,
// description TEXT
// );


void CategoryModel::query(vector<Category>& categorys)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from category");

    // MySql mysql;
    // if (mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if (res != nullptr)
    //     {
    //         MYSQL_ROW row = nullptr;
    //         while ((row = mysql_fetch_row(res)) != nullptr)
    //         {
    //             Category category(
    //                 atoi(row[0]),
    //                 row[1]
    //             );
    //             categorys.push_back(category);
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
            Category category(
                atoi(row[0]),
                row[1]
            );
            categorys.push_back(category);
        }
    }
    mysql_free_result(res);
}