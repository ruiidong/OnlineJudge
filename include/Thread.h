#pragma once
#include "noncopyable.h"

#include <thread>
#include <memory>
#include <functional>
#include <string>
#include <atomic>

using namespace std;

class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc, const string& name = string());
    ~Thread();

    void start();
    void join();

    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const string& name() const { return name_; }

    static int numCreated() { return numCreated_; }

private:
    void setDefaultName();

    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> pthread_;
    pid_t tid_;
    ThreadFunc func_;
    string name_;

    static std::atomic_int numCreated_;
};