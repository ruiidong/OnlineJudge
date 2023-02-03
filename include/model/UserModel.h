#pragma once

#include "model/User.h"

#include <vector>

using namespace std;

class UserModel
{
public:
    bool insert(User &user);
    User query(int uid);
    User query(const string& username, const string& password);
    void query(vector<User>& users);
};