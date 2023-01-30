#pragma once

#include "model/User.h"

class UserModel
{
public:
    bool insert(User &user);
    User query(int uid);
};