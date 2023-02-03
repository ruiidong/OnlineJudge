#include "tinymuduo/net/HttpServer.h"
#include "tinymuduo/net/EventLoop.h"
#include "tinymuduo/net/HttpRequest.h"
#include "tinymuduo/net/HttpResponse.h"
#include "tinymuduo/net/InetAddress.h"
#include "tinymuduo/base/Logging.h"
#include "json/json.h"
#include "Render.h"
#include "Users.h"
#include "model/UserModel.h"
#include "model/ProblemModel.h"
#include "model/CategoryModel.h"

Users users;

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
    if (req.path() == "/login.html")
    {
        string username = req.getData("username");
        string password = req.getData("password");
        Json::Value datas;
        datas["code"] = 1;
        UserModel usermodel;
        User user = usermodel.query(username, password);
        if(user.getUid() != -1)
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
    else if(req.path() == "/register.html")
    {
        string username = req.getData("username");
        string password = req.getData("password");
        string email = req.getData("email");
        Json::Value datas;
        datas["code"] = 1;
        User user(username,password,email);
        UserModel usermodel;
        if(usermodel.insert(user))
        {
            datas["message"] = "注册成功";
            users.add(username, password);
        }
        else
        {
            datas["code"] = 0;
            datas["message"] = "用户存在，注册失败";
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
    else if(req.path()=="/user.html")
    {
        string username = req.query().substr(10);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        string html;
        Render::RenderUser(html, username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    }
    else if(req.path()=="/problemset.html")
    {
        string username = req.query().substr(10);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        ProblemModel problemmodel;
        vector<Problem> problems;
        problemmodel.query(problems);
        string html;
        Render::RenderProblem(html, problems, username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    }
    else if(req.path()=="/category.html")
    {
        string username = req.query().substr(10);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        CategoryModel categorymodel;
        vector<Category> categorys;
        categorymodel.query(categorys);
        string html;
        Render::RenderCategory(html, categorys, username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    }
    else if(req.path()=="/ranklist.html")
    {
        string username = req.query().substr(10);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        UserModel usermodel;
        vector<User> users;
        usermodel.query(users);
        string html;
        Render::RenderRanklist(html,users,username);
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