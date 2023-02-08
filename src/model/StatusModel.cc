#include "model/StatusModel.h"
#include "db/db.h"

void StatusModel::query(const string& username, vector<Status>& statuss)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from status where username = '%s'",username.c_str());

    MySql mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = nullptr;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Status status(
                    atoi(row[0]),
                    row[1],
                    atoi(row[2]),
                    row[3],
                    atoi(row[4]),
                    atoi(row[5]),
                    row[6],
                    atoi(row[7]),
                    row[8]
                );
                statuss.push_back(status);
            }
        }
    }
}