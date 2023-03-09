#pragma once

#include "tinymuduo/base/noncopyable.h"
#include "tinymuduo/net/TcpServer.h"

#include <unordered_map>

class HttpRequest;
class HttpResponse;
class HttpServer : noncopyable
{
public:
    //using HttpCallback = std::function<void(const HttpRequest&,HttpResponse*)>;
    using Handler = std::function<void(const HttpRequest&,HttpResponse*)>;

    HttpServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& name);
    EventLoop* getLoop() const { return server_.getLoop(); }
    // void setHttpCallback(const HttpCallback& cb)
    // {
    //     httpCallback_ = cb;
    // }
    void get(const string& path,const Handler& cb)
    {
        getHandlers_[path] = cb;
    }
    void post(const string& path,const Handler& cb)
    {
        postHandlers_[path] = cb;
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

    void dispatchGet(const HttpRequest& req,HttpResponse* resp);
    void dispatchPost(const HttpRequest& req,HttpResponse* resp);

    string baseDir_;
    TcpServer server_;
    //HttpCallback httpCallback_;
    std::unordered_map<string,Handler> getHandlers_;
    std::unordered_map<string,Handler> postHandlers_;
};
