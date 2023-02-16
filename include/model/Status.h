#pragma once

#include <string>

using namespace std;

class Status
{
public:
    Status(const string& username, int pid, const string result,
            const string& language, const string& submittime)
        : username_(username)
        , pid_(pid)
        , result_(result)
        , language_(language)
        , submittime_(submittime)
    {
    }

    void setSid(int sid)
    {
        sid_ = sid;
    }

    int getSid() const
    {
        return sid_;
    }

    string getUsername() const
    {
        return username_;
    }

    int getPid() const
    {
        return pid_;
    }

    string getResult() const
    {
        return result_;
    }

    void setResult(const string& result)
    {
        result_ = result;
    }

    string getLanguage() const
    {
        return language_;
    }

    string getSubmittime() const
    {
        return submittime_;
    }

private:
    int sid_;
    string username_;
    int pid_;
    string result_;
    string language_;
    string submittime_;
};