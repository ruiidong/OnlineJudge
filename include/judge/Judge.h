#pragma once
#include <string>

using namespace std;

class Problem;
class Judge
{
public:
    virtual bool judge(const string &code, const Problem &problem) = 0;
};