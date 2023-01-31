#pragma once

#include <string>

using namespace std;

class User
{
public:
    User(const string& username = "", const string& password = "", const string& email = "",
            int submit = 0, int solved = 0)
        : uid_(-1)
        , username_(username)
        , password_(password)
        , email_(email)
        , submit_(submit)
        , solved_(solved)
    {
    }
    void setUid(int uid)
    {
        uid_ = uid;
    }
    void setUsername(const string& username)
    {
        username_ = username;
    }
    void setPassword(const string& password)
    {
        password_ = password;
    }
    void setEmail(const string& email)
    {
        email_ = email;
    }
    void setSubmit(int submit)
    {
        submit_ = submit;
    }
    void setSolved(int solved)
    {
        solved_ = solved;
    }
    int getUid()
    {
        return uid_;
    }
    string getUsername()
    {
        return username_;
    }
    string getPassword()
    {
        return password_;
    }
    string getEmail()
    {
        return email_;
    }
    int getSubmit()
    {
        return submit_;
    }
    int getSolved()
    {
        return solved_ ;
    }
private:
    int uid_;
    string username_;
    string password_;
    string email_;
    int submit_;
    int solved_;
};

// CREATE TABLE user(
// uid INT(11) PRIMARY KEY,
// username VARCHAR(20) NOT NULL UNIQUE KEY,
// password VARCHAR(20) NOT NULL,
// email VARCHAR(20),
// submit INT(11),
// solved INT(11)
// );