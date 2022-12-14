#pragma once

#include <string>
#include <netinet/in.h>

using namespace std;

class InetAddress
{
public:
    InetAddress(string ip = "127.0.0.1",uint16_t port = 0);
    explicit InetAddress(const struct sockaddr_in& addr)
        : addr_(addr)
    {}

    sa_family_t family() const { return addr_.sin_family; }
    string toIp() const;
    string toIpPort() const;
    uint16_t port() const;

    const struct sockaddr* getSockAddr() const { return reinterpret_cast<const struct sockaddr*>(&addr_); }
    void setSocketAddrInet(const sockaddr_in& addr) { addr_ = addr; }
private:
    void toIpPort(char* buf,size_t size,const struct sockaddr* addr) const;
    void toIp(char* buf,size_t size,const struct sockaddr* addr) const;

    struct sockaddr_in addr_;
};