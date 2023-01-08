#include <iostream>
#include "Socket.h"
#include "InetAddress.h"

using namespace std;

int main()
{
    InetAddress addr("127.0.0.1",6666);
    Socket socket(::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,0));
    socket.bindAddress(addr);
    socket.listen();
    InetAddress peer;
    int connfd = -1;
    while((connfd = socket.accept(&peer))<0)
    {
    }
    cout << connfd << endl;

    return 0;
}