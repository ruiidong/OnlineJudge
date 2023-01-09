#include "TcpServer.h"
#include "Logging.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
#include "EventLoop.h"

EventLoop* checkLoopNotNull(EventLoop* loop)
{
    if(loop == nullptr)
    {
        LOG_FATAL("loop is nullptr");
    }
    return loop;
}

TcpServer::TcpServer(EventLoop* loop,const InetAddress& listenAddr,const string& nameArg)
    : loop_(checkLoopNotNull(loop))
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

void TcpServer::newConnection(int sockfd,const InetAddress& peerAddr)
{
    EventLoop* ioLoop = threadPool_->getNextLoop();
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