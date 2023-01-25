#pragma once

#include "tinymuduo/base/noncopyable.h"
#include "tinymuduo/net/TcpServer.h"

class HttpRequest;
class HttpResponse;
class HttpServer : noncopyable
{
public:
    using HttpCallback = std::function<void(const HttpRequest&,HttpResponse*)>;

    HttpServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& name);
    EventLoop* getLoop() const { return server_.getLoop(); }
    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }
    void setThreadNum(int numThreads)
    {
        server_.setThreadNum(numThreads);
    }
    void setBaseDir(const string& dir)
    {
        baseDir_ = dir;
    }
    void start();
private:
    void handleFileRequest(const HttpRequest&,HttpResponse*);

    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);
    void onRequest(const TcpConnectionPtr&, const HttpRequest&);

    string baseDir_;
    TcpServer server_;
    HttpCallback httpCallback_;
};
