#pragma once
#include "tinymuduo/base/noncopyable.h"
#include "tinymuduo/net/Channel.h"
#include "tinymuduo/net/Socket.h"

#include <functional>

class EventLoop;
class InetAddress;
class Acceptor : noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd,const InetAddress&)>;

    Acceptor(EventLoop* loop,const InetAddress& listenAddr);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    void listen();

    bool listening() const { return listening_; }

private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;
};