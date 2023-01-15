#include "TcpServer.h"
#include "Logging.h"
#include "TcpConnection.h"
#include "EventLoop.h"

class EchoServer
{
public:
    EchoServer(EventLoop* loop,
                const InetAddress& addr,
                const string& name)
    : loop_(loop)
    , server_(loop,addr,name)
    {
        server_.setConnectionCallback(
            std::bind(&EchoServer::onConnection,this,std::placeholders::_1)
        );
        server_.setMessageCallback(
            std::bind(&EchoServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)
        );
        server_.setThreadNum(3);
    }
    void start()
    {
        server_.start();
    }
    void onConnection(const TcpConnectionPtr& conn)
    {
        if(conn->connected())
        {
            LOG_INFO("conn %s up from %s",conn->name().c_str(),conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("conn %s down from %s",conn->name().c_str(),conn->peerAddress().toIpPort().c_str());
        }
    }
    void onMessage(const TcpConnectionPtr& conn,Buffer* buf,Timestamp time)
    {
        string msg = buf->retrieveAllAsString();
        conn->send(msg);
        conn->shutdown();
    }
private:
    TcpServer server_;
    EventLoop* loop_;
};

int main()
{
    EventLoop loop;
    InetAddress addr("127.0.0.1",6666);
    EchoServer server(&loop,addr,"EchoServer");
    server.start();
    loop.loop();

    return 0;
}