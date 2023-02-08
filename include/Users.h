#pragma once

#include <unordered_map>

using namespace std;

class Users
{
public:
    void earse(const string &username)
    {
        users_.erase(username);
    }
    bool find(const string &username)
    {
        auto it = users_.find(username);
        return it != users_.end() ? true : false;
    }
    void add(const string &username, const string& password)
    {
        users_[username] = password;
    }
private:
    std::unordered_map<string, string> users_;
};