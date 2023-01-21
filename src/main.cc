#include "HttpServer.h"
#include "EventLoop.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "InetAddress.h"
#include "Logging.h"

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
//     //LOG_INFO("---------------------\n%s %s\n-------------------------",req.methodString(),req.path().c_str());
//     if (req.method() == HttpRequest::kGet && req.path() == "/")
//     {
//         resp->setStatusCode(HttpResponse::k200Ok);
//         resp->setStatusMessage("OK");
//         resp->setContentType("text/html");
//         resp->addHeader("Server", "Muduo");
//         string now = "2023/1/19";
//         // resp->setBody("<html><head><title>This is title</title></head>"
//         //     "<body><h1>Hello</h1>Now is " + now +
//         //     "</body></html>");
//         resp->setBody(R"(
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <meta http-equiv="X-UA-Compatible" content="IE=edge">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Document</title>
// </head>
// <body>
    
//     <form action="/login.html" method="post">
//         <label for="username">用户名：</label>
//         <input type="text" name="username" id="username">
     
//         <label for="password">密码：</label>
//         <input type="password" name="password" id="password">

//         <button type="submit">登录</button>
//     </form>
    

// </body>
// </html>
//         )");
//     }
//     else if(req.method() == HttpRequest::kPost && req.path() == "/login.html")
//     {
//         resp->setStatusCode(HttpResponse::k200Ok);
//         resp->setStatusMessage("OK");
//         resp->setContentType("text/html");
//         resp->addHeader("Server", "Muduo");
//         resp->setBody(R"(
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <meta http-equiv="X-UA-Compatible" content="IE=edge">
//     <meta name="viewport" content="width=device-width, initial-scale=1.0">
//     <title>Document</title>
// </head>
// <body>
    
// login success
    

// </body>
// </html>
//         )");
//     }
}

int main()
{
    EventLoop loop;
    InetAddress addr("192.168.230.152",8000);
    HttpServer server(&loop,addr,"dummy");
    server.setHttpCallback(onRequest);
    server.setThreadNum(3);
    server.setBaseDir("../template");
    server.start();
    loop.loop();

    return 0;
}