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
#include "model/StatusModel.h"
#include "judge/JudgeFactory.h"

#include <memory>

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
    else if(req.path()=="/logout.html")
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

        users.earse(username);

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
        Render::RenderProblemSet(html, problems, username);
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
    else if(req.path()=="/status.html")
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

        StatusModel statusmodel;
        vector<Status> status;
        statusmodel.query(username, status);
        string html;
        Render::RenderStatus(html,status,username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    }
    else if(req.path() == "/problem.html")
    {
        string usernamepid = req.query().substr(10);
        int idx = usernamepid.find('?');
        string username = usernamepid.substr(0,idx);
        string pid = usernamepid.substr(idx+5);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        ProblemModel problemodel;
        Problem problem = problemodel.query(stoi(pid));
        string html;
        Render::RenderProblem(html, problem, username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    }
    else if(req.path() == "/compile.html")
    {
        string usernamepid = req.query().substr(10);
        int idx = usernamepid.find('?');
        string username = usernamepid.substr(0,idx);
        string pid = usernamepid.substr(idx+5);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        string code = req.getData("code");
        string codeType = req.getData("code_type");
        
        ProblemModel problemodel;
        Problem problem = problemodel.query(stoi(pid));
        problem.setSubmit(problem.getSubmit()+1);
        problemodel.updateSubmit(problem);
        
        UserModel usermodel;
        User user = usermodel.query(username);
        user.setSubmit(user.getSubmit()+1);
        usermodel.updateSubmit(user);

        Json::Value datas;
        datas["message"] = "Accepted";
        Status status(username, stoi(pid), "Accepted", codeType, Timestamp::now().toString());

        // Judge* judge = nullptr;
        // if(codeType == "c/c++")
        // {
        //     judge = new CppJudge;
        // }
        // else if(codeType == "python")
        // {
        //     judge = new PyJudge;
        // }
        // if(judge->judge(code, problem)==false)
        // {
        //     datas["message"] = "Error";
        //     status.setResult("Error");
            
        //     problem.setSolved(problem.getSolved()-1);
        //     problemodel.updateSolved(problem);

        //     user.setSolved(user.getSolved()-1);
        //     usermodel.updateSolved(user);
        // }

        std::unique_ptr<JudgeFactory> judgeFactory(new JudgeFactory());
        std::unique_ptr<Judge> judge(judgeFactory->createJudge(codeType));
        if(judge->judge(code, problem)==false)
        {
            datas["message"] = "Error";
            
            status.setResult("Error");
            
            problem.setSolved(problem.getSolved()-1);
            problemodel.updateSolved(problem);

            user.setSolved(user.getSolved()-1);
            usermodel.updateSolved(user);
        }

        problem.setSolved(problem.getSolved()+1);
        problemodel.updateSolved(problem);
        
        user.setSolved(user.getSolved()+1);
        usermodel.updateSolved(user);

        StatusModel statusmodel;
        statusmodel.insert(status);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(datas.toStyledString());
    }
}

int main()
{
    EventLoop loop;
    InetAddress addr("192.168.230.152",8000);
    HttpServer server(&loop,addr,"dummy");
    //server.setHttpCallback(onRequest);
    server.post("/login.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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
    });
    server.post("/register.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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
    });
    server.get("/logout.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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

        users.earse(username);

        resp->setStatusCode(HttpResponse::k302MovedTemporarily);
        resp->setStatusMessage("OK");
        resp->setRedirect("/");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo"); 
    });
    server.get("/user.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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
    });
    server.get("/problemset.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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
        Render::RenderProblemSet(html, problems, username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    });
    server.get("/ranklist.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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
    });
    server.get("/status.html",[](const HttpRequest &req, HttpResponse *resp)->void{
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

        StatusModel statusmodel;
        vector<Status> status;
        statusmodel.query(username, status);
        string html;
        Render::RenderStatus(html,status,username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    });
    server.get("/problem.html",[](const HttpRequest &req, HttpResponse *resp)->void{
        string usernamepid = req.query().substr(10);
        int idx = usernamepid.find('?');
        string username = usernamepid.substr(0,idx);
        string pid = usernamepid.substr(idx+5);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        ProblemModel problemodel;
        Problem problem = problemodel.query(stoi(pid));
        string html;
        Render::RenderProblem(html, problem, username);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(html);
    });
    server.post("/compile.html",[](const HttpRequest &req, HttpResponse *resp)->void{
        string usernamepid = req.query().substr(10);
        int idx = usernamepid.find('?');
        string username = usernamepid.substr(0,idx);
        string pid = usernamepid.substr(idx+5);

        if(!users.find(username))
        {
            resp->setStatusCode(HttpResponse::k302MovedTemporarily);
            resp->setStatusMessage("OK");
            resp->setRedirect("/");
            resp->setContentType("text/html");
            resp->addHeader("Server", "Muduo");

            return;
        }

        string code = req.getData("code");
        string codeType = req.getData("code_type");
        
        ProblemModel problemodel;
        Problem problem = problemodel.query(stoi(pid));
        problem.setSubmit(problem.getSubmit()+1);
        problemodel.updateSubmit(problem);
        
        UserModel usermodel;
        User user = usermodel.query(username);
        user.setSubmit(user.getSubmit()+1);
        usermodel.updateSubmit(user);

        Json::Value datas;
        datas["message"] = "Accepted";
        Status status(username, stoi(pid), "Accepted", codeType, Timestamp::now().toString());

        // Judge* judge = nullptr;
        // if(codeType == "c/c++")
        // {
        //     judge = new CppJudge;
        // }
        // else if(codeType == "python")
        // {
        //     judge = new PyJudge;
        // }
        // if(judge->judge(code, problem)==false)
        // {
        //     datas["message"] = "Error";
        //     status.setResult("Error");
            
        //     problem.setSolved(problem.getSolved()-1);
        //     problemodel.updateSolved(problem);

        //     user.setSolved(user.getSolved()-1);
        //     usermodel.updateSolved(user);
        // }

        std::unique_ptr<JudgeFactory> judgeFactory(new JudgeFactory());
        std::unique_ptr<Judge> judge(judgeFactory->createJudge(codeType));
        if(judge->judge(code, problem)==false)
        {
            datas["message"] = "Error";
            
            status.setResult("Error");
            
            problem.setSolved(problem.getSolved()-1);
            problemodel.updateSolved(problem);

            user.setSolved(user.getSolved()-1);
            usermodel.updateSolved(user);
        }

        problem.setSolved(problem.getSolved()+1);
        problemodel.updateSolved(problem);
        
        user.setSolved(user.getSolved()+1);
        usermodel.updateSolved(user);

        StatusModel statusmodel;
        statusmodel.insert(status);
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "Muduo");
        resp->setBody(datas.toStyledString());
    });
    server.setThreadNum(3);
    server.setBaseDir("../template");
    server.start();
    loop.loop();

    return 0;
}
