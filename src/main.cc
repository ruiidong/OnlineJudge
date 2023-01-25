#include "tinymuduo/net/HttpServer.h"
#include "tinymuduo/net/EventLoop.h"
#include "tinymuduo/net/HttpRequest.h"
#include "tinymuduo/net/HttpResponse.h"
#include "tinymuduo/net/InetAddress.h"
#include "tinymuduo/base/Logging.h"
#include "json/json.h"
#include "Render.h"
#include "Users.h"
#include "db/db.h"

Users users;

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
    if (req.path() == "/login.html")
    {
        string username = req.getData("username");
        string password = req.getData("password");
        // LOG_INFO("---------------------");
        // LOG_INFO("%s %s",username.c_str(),password.c_str());
        // LOG_INFO("---------------------");
        Json::Value datas;
        datas["code"] = 1;
        if(username=="admin" && password=="admin")
        {
            datas["message"] = "登录成功";
            users.add(username, password);
        }
        else
        {
            datas["code"] = 0;
            datas["message"] = "用户名或密码错误";
        }
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(datas.toStyledString());
    }
    else if(req.path()=="/logout.html" && req.query()=="?username=admin")
    {
        users.earse("admin");

        resp->setStatusCode(HttpResponse::k302MovedTemporarily);
        resp->setStatusMessage("OK");
        resp->setRedirect("/");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo"); 
    }
    else if(req.path()=="/user.html" && req.query()=="?username=admin")
    {
        if(!users.find("admin"))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        string html;
        Render::RenderUser(html);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    }
//     if (req.method() == HttpRequest::kGet && req.path() == "/")
//     {
//         resp->setStatusCode(HttpResponse::k200Ok);
//         resp->setStatusMessage("OK");
//         resp->setContentType("text/html");
//         resp->addHeader("Server", "Muduo");
//         string now = "2023/1/19";
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
}

int main()
{
    {
        MySql mysql;
    }
    
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