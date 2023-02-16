#pragma once

#include "model/Status.h"

#include <vector>

using namespace std;

class StatusModel
{
public:
    void query(const string& username, vector<Status>& status);
    void insert(Status& status);
};