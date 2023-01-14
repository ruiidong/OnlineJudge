#include "TcpServer.h"
#include "Logging.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpConnection.h"

#include <string.h>

EventLoop* checkLoopNotNullInTcpServer(EventLoop* loop)
{
    if(loop == nullptr)
    {
        LOG_FATAL("loop is nullptr");
    }
    return loop;
}

TcpServer::TcpServer(EventLoop* loop,const InetAddress& listenAddr,const string& nameArg)
    : loop_(checkLoopNotNullInTcpServer(loop))
    , ipPort_(listenAddr.toIpPort())
    , name_(nameArg)
    , acceptor_(new Acceptor(loop,listenAddr))
    , threadPool_(new EventLoopThreadPool(loop,name_))
    , connectionCallback_()
    , messageCallback_()
    , nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection,this,std::placeholders::_1,std::placeholders::_2)
    );
}

void TcpServer::setThreadNum(int numThreads)
{
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
    if(started_++ == 0)
    {
        loop_->runInLoop(
            std::bind(&Acceptor::listen,acceptor_.get())
        );
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
    EventLoop* ioLoop = threadPool_->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof buf, "-%s#%d",ipPort_.c_str(),nextConnId_);
    ++nextConnId_;
    string connName = name_ + buf;

    LOG_INFO("TcpServer::newConnection [%s] - new connection [%s] from %s",name_.c_str(),connName.c_str(),peerAddr.toIpPort().c_str());
    
    sockaddr_in local;
    ::bzero(&local, sizeof local);
    socklen_t addrlen = sizeof local;
    if(::getsockname(sockfd, reinterpret_cast<sockaddr*>(&local), &addrlen) < 0)
    {
        LOG_ERROR("sockets::getLocalAddr");
    }
    InetAddress localAddr(local);
    TcpConnectionPtr conn(new TcpConnection(
        ioLoop,
        connName,
        sockfd,
        localAddr,
        peerAddr
    ));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(
        std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)
    );
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInlLoop, this, conn));
}

void TcpServer::removeConnectionInlLoop(const TcpConnectionPtr& conn)
{
    LOG_INFO("TcpServer::removeConnectionInLoop [%s] - connection %s",name_.c_str(),conn->name().c_str());
    connections_.erase(conn->name());
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(
        std::bind(&TcpConnection::connectDestroyed, conn)
    );
}