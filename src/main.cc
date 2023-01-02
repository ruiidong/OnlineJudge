#include <iostream>
#include "Thread.h"

using namespace std;

int main()
{
    Thread thread(std::bind([](int a){
        cout << "Hello World!!! " << a << endl;
    },1),"Test");

    thread.start();

    thread.join();

    return 0;
}