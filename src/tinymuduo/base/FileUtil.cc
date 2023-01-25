#include "tinymuduo/base/FileUtil.h"

#include <sys/stat.h>
#include <fstream>
#include <regex>

bool FileUtil::isFile(const string& path)
{
    struct stat st;
    return stat(path.c_str(), &st) >=0 && S_ISREG(st.st_mode);
}

void FileUtil::readFile(const string& path, string& out)
{
    std::ifstream fs(path, std::ios_base::binary);
    fs.seekg(0, std::ios_base::end);
    auto size = fs.tellg();
    fs.seekg(0);
    out.resize(size);
    fs.read(&out[0], size);
}

std::string fileExtension(const string& path)
{
    std::smatch m;
    auto pat = std::regex("\\.([a-zA-Z0-9]+)$");
    if(std::regex_search(path, m, pat))
    {
        return m[1].str();
    }
    return string();
}

const char* FileUtil::findContentType(const string& path)
{
    auto ext = fileExtension(path);
    if (ext == "txt") {
        return "text/plain";
    } else if (ext == "html") {
        return "text/html";
    } else if (ext == "css") {
        return "text/css";
    } else if (ext == "jpeg" || ext == "jpg") {
        return "image/jpg";
    } else if (ext == "png") {
        return "image/png";
    } else if (ext == "gif") {
        return "image/gif";
    } else if (ext == "svg") {
        return "image/svg+xml";
    } else if (ext == "ico") {
        return "image/x-icon";
    } else if (ext == "json") {
        return "application/json";
    } else if (ext == "pdf") {
        return "application/pdf";
    } else if (ext == "js") {
        return "application/javascript";
    } else if (ext == "xml") {
        return "application/xml";
    } else if (ext == "xhtml") {
        return "application/xhtml+xml";
    }
    return nullptr;
}