#pragma once

#include "model/Problem.h"
#include <vector>

using namespace std;

class ProblemModel
{
public:
    void query(vector<Problem>& problems);
    Problem query(int pid);
    void updateSubmit(Problem& problem);
    void updateSolved(Problem& problem);
};