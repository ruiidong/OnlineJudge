#pragma once

#include <string>

using namespace std;

class Category
{
public:
    Category(int cid, const string& text)
        : cid_(cid)
        , text_(text)
    {
    }
    int getCid() const 
    {
        return cid_;
    }
    string getText() const 
    {
        return text_;
    }
private:
    int cid_;
    string text_;
};