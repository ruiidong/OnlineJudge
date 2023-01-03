#include <iostream>
#include "EventLoopThread.h"

using namespace std;

int main()
{
    EventLoopThread loop_(EventLoopThread::ThreadInitCallback(),"loop1");
    loop_.startLoop();

    return 0;
}