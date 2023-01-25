#pragma once
#include "tinymuduo/base/noncopyable.h"
#include "tinymuduo/net/Callbacks.h"

#include <string>
#include <memory>
#include <functional>
#include <atomic>
#include <unordered_map>

using namespace std;

class EventLoop;
class Acceptor;
class EventLoopThreadPool;
class InetAddress;
class TcpServer : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    TcpServer(EventLoop* loop,const InetAddress& listenAddr,const string& nameArg);
    ~TcpServer();

    const string& name() const { return name_; }
    EventLoop* getLoop() const { return loop_; }

    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback& cb)
    { threadInitCallback_ = cb; }

    void start();

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }

    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    { writeCompleteCallback_ = cb; }

private:
    void newConnection(int sockfd,const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInlLoop(const TcpConnectionPtr& conn);
    
    using ConnectionMap = std::unordered_map<string,TcpConnectionPtr>;

    EventLoop* loop_;
    const string ipPort_;
    const string name_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;
    std::atomic_int started_;

    int nextConnId_;
    ConnectionMap connections_;
};