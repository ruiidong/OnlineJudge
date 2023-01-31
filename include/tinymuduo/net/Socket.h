#pragma once
#include "tinymuduo/base/noncopyable.h"

class InetAddress;
class Socket : noncopyable
{
public:
    explicit Socket(int sockfd)
        : sockfd_(sockfd)
    { }
    ~Socket();

    int fd() const { return sockfd_; }

    void bindAddress(const InetAddress& localaddr);
    void listen();

    int accept(InetAddress* peeraddr);

    void shutdownWrite();

    void setKeepAlive(bool on);
private:
    const int sockfd_;
};