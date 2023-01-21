#pragma once

#include <string>

using namespace std;

namespace FileUtil
{
bool isFile(const string& path);

void readFile(const string& path, string& out);

const char* findContentType(const string& path);
};