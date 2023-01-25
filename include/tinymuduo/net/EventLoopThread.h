#pragma once
#include "tinymuduo/base/noncopyable.h"
#include "tinymuduo/base/Thread.h"

#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

using namespace std;

class EventLoop;
class EventLoopThread : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
            const string& name = string());
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

    EventLoop* loop_;
    Thread thread_;
    bool exiting_;
    std::mutex mutex_;
    condition_variable cond_;
    ThreadInitCallback callback_;
};