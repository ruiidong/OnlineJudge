#include "tinymuduo/net/Socket.h"
#include "tinymuduo/net/InetAddress.h"
#include "tinymuduo/base/Logging.h"
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

Socket::~Socket()
{
    ::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& localaddr)
{
    int ret = ::bind(sockfd_,localaddr.getSockAddr(),sizeof(sockaddr_in));
    if(ret < 0)
    {
        LOG_FATAL("Socket::bindAddress");
    }
}

void Socket::listen()
{
    int ret = ::listen(sockfd_,SOMAXCONN);
    if(ret < 0)
    {
        LOG_FATAL("Socket::listen");
    }
}

int Socket::accept(InetAddress* peeraddr)
{
    sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    ::bzero(&addr,sizeof addr);
    int connfd = ::accept4(sockfd_,reinterpret_cast<sockaddr*>(&addr),&addrlen,SOCK_NONBLOCK|SOCK_CLOEXEC);
    if(connfd < 0)
    {
        int savedErrno = errno;
        if(savedErrno!=EAGAIN)
        {
            LOG_ERROR("Socket::accept");
        }
    }
    else
    {
        peeraddr->setSocketAddrInet(addr);
    }
    return connfd;
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                &optval,sizeof optval);
}

void Socket::shutdownWrite()
{
    if(::shutdown(sockfd_, SHUT_WR) < 0)
    {
        LOG_ERROR("Socket::shutdownWrite");
    }
}