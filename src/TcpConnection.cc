#include "TcpConnection.h"
#include "Logging.h"
#include "Socket.h"
#include "Channel.h"

#include <functional>

EventLoop* checkLoopNotNull(EventLoop* loop)
{
    if(loop == nullptr)
    {
        LOG_FATAL("loop is nullptr");
    }
    return loop;
}

TcpConnection::TcpConnection(EventLoop* loop,
                        const string& nameArg,
                        int sockfd,
                        const InetAddress& localAddr,
                        const InetAddress& peerAddr)
    : loop_(checkLoopNotNull(loop))
    , name_(nameArg)
    , state_(kConnecting)
    , reading_(true)
    , socket_(new Socket(sockfd))
    , channel_(new Channel(loop, sockfd))
    , localAddr_(localAddr)
    , peerAddr_(peerAddr)
    , highWaterMark_(64*1024*1024)
{
    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead,this,std::placeholders::_1)
    );
    channel_->setWriteCallback(
        std::bind(&TcpConnection::handleWrite,this)
    );
    channel_->setCloseCallback(
        std::bind(&TcpConnection::handleClose,this)
    );
    channel_->setErrorCallback(
        std::bind(&TcpConnection::handleError,this)
    );
    LOG_DEBUG("TcpConnection::ctor[%s] at %p fd=%d",
                name_,this,sockfd);
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG("TcpConnection::dtor[%s] at fd=%d state=%d",
                name_,channel_->fd(),state_);
}