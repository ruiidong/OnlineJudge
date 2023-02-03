#pragma once

#include <string>

using namespace std;

class Problem
{
public:
    Problem(int pid, const string& title,const string& description, const string& apps, const string& input,
            const string& output, const string& sampleInput, const string& sampleOutput,
            int submit, int solved)
        : pid_(pid)
        , title_(title)
        , description_(description)
        , apps_(apps)
        , input_(input)
        , output_(output)
        , sampleInput_(sampleInput)
        , sampleOutput_(sampleOutput)
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
    string getInput() const
    {
        return input_;
    }
    string getOutput() const
    {
        return output_;
    }
    string getSampleInput() const
    {
        return sampleInput_;
    }
    string getSampleOutput() const
    {
        return sampleOutput_;
    }
private:
    int pid_;
    string title_;
    string description_;
    string apps_;
    string input_;
    string output_;
    string sampleInput_;
    string sampleOutput_;
    int submit_;
    int solved_;
};