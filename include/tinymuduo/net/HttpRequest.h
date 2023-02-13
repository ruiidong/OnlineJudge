#pragma once

#include "tinymuduo/base/Timestamp.h"

#include <string>
#include <unordered_map>

using namespace std;

class HttpRequest
{
public:
    enum Method
    {
        kInvalid, kGet, kPost, kHead, kPut, kDelete
    };
    enum Version
    {
        kUnknown, kHttp10, kHttp11
    };
    HttpRequest()
        : method_(kInvalid)
        , version_(kUnknown)
    {
    }
    void setVersion(Version v)
    {
        version_ = v;
    }
    Version getVersion() const
    {
        return version_;
    }
    bool setMethod(const char* start, const char* end)
    {
        string m(start, end);
        if(m == "GET")
        {
            method_ = kGet;
        }
        else if(m == "POST")
        {
            method_ = kPost;
        }
        else if(m == "HEAD")
        {
            method_ = kHead;
        }
        else if(m == "PUT")
        {
            method_ = kPut;
        }
        else if(m == "DELETE")
        {
            method_ = kDelete;
        }
        else
        {
            method_ = kInvalid;
        }
        return method_ != kInvalid;
    }
    Method method() const
    {
        return method_;
    }
    const char* methodString() const
    {
        const char* result = "UNKNOWN";
        switch (method_)
        {
        case kGet:
            result = "GET";
            break;
        case kPost:
            result = "POST";
            break;
        case kHead:
            result = "HEAD";
            break;
        case kPut:
            result = "PUT";
            break;
        case kDelete:
            result = "DELETE";
            break;
        default:
            break;
        }
        return result;
    }
    void setPath(const char* start,const char* end)
    {
        path_.assign(start,end);
    }
    const string& path() const
    {
        return path_;
    }
    void setQuery(const char* start,const char* end)
    {
        query_.assign(start, end);
    }
    const string& query() const
    {
        return query_;
    }
    void setReceiveTime(Timestamp t)
    {
        receiveTime_ = t;
    }
    Timestamp receiveTime() const
    {
        return receiveTime_;
    }
    void addHeader(const char* start, const char* colon, const char* end)
    {
        string field(start, colon);
        ++colon;
        while(colon < end && isspace(*colon))
        {
            ++colon;
        }
        string value(colon, end);
        while(!value.empty() && isspace(value[value.size() - 1]))
        {
            value.resize(value.size()-1);
        }
        headers_[field] = value;
    }
    void setRemoteAddr(const string& ip)
    {
        ip_ = ip;
    }
    string getRemoteAddr()
    {
        return ip_;
    }
    void setCookie(const string& cookie)
    {
        cookie_ = cookie;
    }
    string getCookie()
    {
        return cookie_; 
    }
    void addData(const char* start, const char* equal, const char* ands)
    {
        string field(start, equal);
        ++equal;
        string value(equal, ands);
        string decode = urlDecode(value);
        datas_[field] = decode;
    }
    string getHeader(const string& field) const
    {
        string result;
        auto it = headers_.find(field);
        if(it != headers_.end())
        {
            result = it->second;
        }
        return result;
    }
    string getData(const string& field) const
    {
        string result;
        auto it = datas_.find(field);
        if(it != datas_.end())
        {
            result = it->second;
        }
        return result;
    }
    const std::unordered_map<string, string>& headers() const
    {
        return headers_;
    }
    void swap(HttpRequest& that)
    {
        std::swap(method_, that.method_);
        std::swap(version_, that.version_);
        path_.swap(that.path_);
        query_.swap(that.query_);
        receiveTime_.swap(that.receiveTime_);
        headers_.swap(that.headers_);
    }
private:
    unsigned char fromHex(unsigned char x) 
    { 
        unsigned char y;
        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
        else if (x >= '0' && x <= '9') y = x - '0';
        //else assert(0);
        return y;
    }

    string urlDecode(const string& str)
    {
        std::string strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            if (str[i] == '+') strTemp += ' ';
            else if (str[i] == '%')
            {
                unsigned char high = fromHex((unsigned char)str[++i]);
                unsigned char low = fromHex((unsigned char)str[++i]);
                strTemp += high*16 + low;
            }
            else strTemp += str[i];
        }
        return strTemp;
    }

    Method method_;
    Version version_;
    string path_;
    string query_;
    Timestamp receiveTime_;
    std::unordered_map<string, string> headers_;
    std::unordered_map<string, string> datas_;
    string ip_;
    string cookie_;
};