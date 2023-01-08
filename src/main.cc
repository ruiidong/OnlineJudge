#include <iostream>
#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"

using namespace std;

int main()
{
    EventLoop loop;
    InetAddress addr("127.0.0.1",6666);
    Acceptor acceptor(&loop,addr);
    acceptor.setNewConnectionCallback(
        [](int connfd,const InetAddress& peer)->void{
            cout << "connfd : " << connfd << endl;
            cout << "peer : " << peer.toIpPort() << endl;
        }
    );
    acceptor.listen();
    loop.loop();

    return 0;
}