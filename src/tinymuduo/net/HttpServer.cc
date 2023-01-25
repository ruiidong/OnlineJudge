#include "tinymuduo/net/HttpServer.h"
#include "tinymuduo/net/HttpRequest.h"
#include "tinymuduo/net/HttpResponse.h"
#include "tinymuduo/net/TcpConnection.h"
#include "tinymuduo/base/FileUtil.h"

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop* loop,
        const InetAddress& listenAddr,
        const string& name)
    : server_(loop, listenAddr, name)
    , httpCallback_(defaultHttpCallback)
{
    server_.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, std::placeholders::_1)
    );
    server_.setMessageCallback(
        std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
    HttpContext* context = conn->getContext();

    if(!context->parseRequest(buf, receiveTime))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if(context->gotAll())
    {
        HttpRequest& req = context->request();
        req.setRemoteAddr(conn->peerAddress().toIp());
        
        onRequest(conn, req);
        context->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    const string& connection = req.getHeader("Connection");
    bool close = connection=="close" || (req.getVersion()==HttpRequest::kHttp10 && connection!="Keep-Alive");

    HttpResponse response(close);
    handleFileRequest(req, &response);
    httpCallback_(req, &response);
    
    Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(&buf);
    if(response.closeConnection())
    {
        conn->shutdown();
    }
}

void HttpServer::handleFileRequest(const HttpRequest& req,HttpResponse* resp)
{
    string path = baseDir_ + req.path();
    if(req.path() == "/")
    {
        path += "index.html";
    }
    
    if(FileUtil::isFile(path))
    {
        string data;
        FileUtil::readFile(path, data);        
        resp->setBody(data);
        auto type = FileUtil::findContentType(path);
        if(type)
        {
            resp->setContentType(type);
        }
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
    }
}