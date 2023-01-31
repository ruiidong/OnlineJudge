#include "model/UserModel.h"
#include "db/db.h"

#include <iostream>

// CREATE TABLE user(
// uid INT(11) PRIMARY KEY AUTO_INCREMENT,
// username VARCHAR(20) NOT NULL UNIQUE KEY,
// password VARCHAR(20) NOT NULL,
// email VARCHAR(20),
// submit INT(11),
// solved INT(11)
// );

bool UserModel::insert(User &user)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into user(username,password,email,submit,solved) values('%s','%s','%s',%d,%d)",
                user.getUsername().c_str(),user.getPassword().c_str(),user.getEmail().c_str(),user.getSubmit(),user.getSolved());

    MySql mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            user.setUid(mysql_insert_id(mysql.getConn()));
            return true;
        }
    }
    return false;
}

User UserModel::query(int uid)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from user where uid = %d", uid);

    MySql mysql;
    if(mysql.connect())
    {
        MYSQL_RES* res = mysql.query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                User user;
                user.setUid(atoi(row[0]));
                user.setUsername(row[1]);
                user.setPassword(row[2]);
                user.setEmail(row[3]);
                user.setSubmit(atoi(row[4]));
                user.setSolved(atoi(row[5]));
                return user;
            }
        }
    }
    return User();
}

User UserModel::query(const string& username, const string& password)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from user where username = '%s' and password = '%s'", username.c_str(), password.c_str());

    MySql mysql;
    if(mysql.connect())
    {
        MYSQL_RES* res = mysql.query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                User user;
                user.setUid(atoi(row[0]));
                user.setUsername(row[1]);
                user.setPassword(row[2]);
                user.setEmail(row[3]);
                user.setSubmit(atoi(row[4]));
                user.setSolved(atoi(row[5]));
                return user;
            }
        }
    }
    return User();
}