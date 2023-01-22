#pragma once

#include <unordered_map>

using namespace std;

class Users
{
public:
    void earse(const string &user_id)
    {
        users_.erase(user_id);
    }
    bool find(const string &user_id)
    {
        auto it = users_.find(user_id);
        return it != users_.end() ? true : false;
    }
    void add(const string &user_id, const string& password)
    {
        users_[user_id] = password;
    }
private:
    std::unordered_map<string, string> users_;
};