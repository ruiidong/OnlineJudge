#pragma once

#include <string>

using namespace std;

class Status
{
public:
    Status(int sid, const string& username, int pid, const string result,
            int memorylimit, int timelimit, const string& language, int length, const string& submittime)
        : sid_(sid)
        , username_(username)
        , pid_(pid)
        , result_(result)
        , memorylimit_(memorylimit_)
        , timelimit_(timelimit)
        , language_(language)
        , length_(length)
        , submittime_(submittime)
    {
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

    int getMemoryLimit() const
    {
        return memorylimit_;
    }

    int getTimeLimit() const
    {
        return timelimit_;
    }

    string getLanguage() const
    {
        return language_;
    }

    int getLength() const
    {
        return length_;
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
    int memorylimit_;
    int timelimit_;
    string language_;
    int length_;
    string submittime_;
};