#include "Acceptor.h"
#include "Logging.h"
#include "InetAddress.h"
#include <arpa/inet.h>

int createNonblockingOrDie(sa_family_t family)
{
    int sockfd = ::socket(family,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0)
    {
        LOG_FATAL("Acceptor::createNonblockingOrDie");
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop* loop,const InetAddress& listenAddr)
    : loop_(loop)
    , acceptSocket_(createNonblockingOrDie(listenAddr.family()))
    , acceptChannel_(loop,acceptSocket_.fd())
    , listening_(false)
{
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(
        std::bind(&Acceptor::handleRead,this)
    );
}

Acceptor::~Acceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::listen()
{
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if(connfd >= 0)
    {
        if(newConnectionCallback_)
        {
            newConnectionCallback_(connfd,peerAddr);
        }
        else
        {
            ::close(connfd);
        }
    }
    else
    {
        LOG_ERROR("in Acceptor::handleRead");
    }
}