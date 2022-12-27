#include "Channel.h"
#include "EventLoop.h"
#include <poll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop,int fd)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , revents_(0)
    , index_(-1)
    , tied_(false)
    , eventHandling_(false)
    , addedToLoop_(false)
{
}

Channel::~Channel()
{
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
    tie_ = obj;
    tied_ = true;
}

void Channel::update()
{
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

void Channel::remove()
{
    addedToLoop_ = false;
    loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    std::shared_ptr<void> guard;
    if(tied_)
    {
        guard = tie_.lock();
        if(guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    eventHandling_ = true;
    if((revents_ & POLLHUP) && !(revents_ & POLLIN))
    {
        if(closeCallback_)
        {
            closeCallback_();
        }
    }

    if(revents_ & (POLLERR | POLLNVAL))
    {
        if(errorCallback_)
        {
            errorCallback_();
        }
    }

    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if(revents_ & POLLOUT)
    {
        if(writeCallback_)
        {
            writeCallback_();
        }
    }
    eventHandling_ = false;
}