#include "HttpServer.h"
#include "EventLoop.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "InetAddress.h"

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
    if (req.path() == "/")
    {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        string now = "2023/1/19";
        resp->setBody("<html><head><title>This is title</title></head>"
            "<body><h1>Hello</h1>Now is " + now +
            "</body></html>");
    }
}

int main()
{
    EventLoop loop;
    InetAddress addr("192.168.230.152",8000);
    HttpServer server(&loop,addr,"dummy");
    server.setHttpCallback(onRequest);
    server.setThreadNum(3);
    server.start();
    loop.loop();

    return 0;
}