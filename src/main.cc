#include <iostream>
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

using namespace std;

int main()
{
    EventLoop loop;
    cout << "baseLoop " << &loop << endl;
    EventLoopThreadPool pool(&loop,"pool");
    pool.setThreadNum(3);
    cout << "subLoop" << endl;
    pool.start();
    for(int i = 0;i < 3;i++)
    {
        cout << pool.getNextLoop() << endl;        
    }

    return 0;
}