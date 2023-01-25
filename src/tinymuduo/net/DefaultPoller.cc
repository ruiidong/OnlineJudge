#include "tinymuduo/net/Poller.h"
#include "tinymuduo/net/EPollPoller.h"

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return new EPollPoller(loop);
}