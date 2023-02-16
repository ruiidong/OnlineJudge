#pragma once

#include <string>

using namespace std;

class Problem
{
public:
    Problem(int pid, const string& title,const string& description, const string& apps,
            int submit, int solved)
        : pid_(pid)
        , title_(title)
        , description_(description)
        , apps_(apps)
        , submit_(submit)
        , solved_(solved)
    {
    }
    int getPid() const
    {
        return pid_;
    }
    int getSubmit() const
    {
        return submit_;
    }
    int getSolved() const
    {
        return solved_;
    }
    void setSubmit(int submit)
    {
        submit_ = submit;
    }
    void setSolved(int solved)
    {
        solved_ = solved;
    }
    string getDescription() const
    {
        return description_;
    }
    string getTitle() const
    {
        return title_;
    }
    string getApps() const
    {
        return apps_;
    }
private:
    int pid_;
    string title_;
    string description_;
    string apps_;
    int submit_;
    int solved_;
};