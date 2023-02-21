#pragma once
#include <string>
#include "CppJudge.h"
#include "PyJudge.h"

using namespace std;

class Judge;
class JudgeFactory
{
public:
    Judge* createJudge(const string& codeType)
    {
        if(codeType == "c/c++")
        {
            return new CppJudge;
        }
        else if(codeType == "python")
        {
            return new PyJudge;
        }
        else
        {
            return nullptr;
        }
    }
};