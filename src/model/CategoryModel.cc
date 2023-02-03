#include "model/CategoryModel.h"

#include "db/db.h"

// CREATE TABLE category(
// cid INT(11) PRIMARY KEY AUTO_INCREMENT,
// description TEXT
// );


void CategoryModel::query(vector<Category>& categorys)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from category");

    MySql mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
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
    }
}