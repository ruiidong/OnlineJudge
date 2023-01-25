#include "tinymuduo/net/InetAddress.h"
#include "tinymuduo/base/Logging.h"

#include <endian.h>
#include <arpa/inet.h>
#include <string.h>

//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

InetAddress::InetAddress(string ip, uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htobe16(port);
    if (::inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) <= 0)
    {
        LOG_ERROR("InetAddress::InetAddress");
    }
}

string InetAddress::toIp() const
{
    char buf[64] = "";
    toIp(buf,sizeof buf,getSockAddr());
    return buf;
}

string InetAddress::toIpPort() const
{
    char buf[64] = "";
    toIpPort(buf,sizeof buf,getSockAddr());
    return buf;
}

uint16_t InetAddress::port() const
{
    return be16toh(addr_.sin_port);
}

void InetAddress::toIpPort(char* buf,size_t size,const struct sockaddr* addr) const 
{
    toIp(buf,size,addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in* addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
    uint16_t port = be16toh(addr4->sin_port);
    snprintf(buf+end,size-end,":%u",port);
}

void InetAddress::toIp(char* buf,size_t size,const struct sockaddr* addr) const
{
    const struct sockaddr_in* addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
    ::inet_ntop(AF_INET,&addr4->sin_addr,buf,static_cast<socklen_t>(size));
}