#include "EPollPoller.h"
#include "Logging.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <strings.h>

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

EPollPoller::EPollPoller(EventLoop* loop)
    : Poller(loop)
    , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , events_(kInitEventListSize)
{
    if(epollfd_ < 0)
    {
        LOG_FATAL("EPollPoller::EPollPoller");
    }
}

EPollPoller::~EPollPoller()
{
    ::close(epollfd_);
}

Timestamp EPollPoller::poll(int timeoutMs,ChannelList* activeChannels)
{
    LOG_INFO("fd total count %lu",channels_.size());
    int numEvents = ::epoll_wait(epollfd_,
                                 &*events_.begin(),
                                 static_cast<int>(events_.size()),
                                 timeoutMs);
    int savedErrno = errno;
    Timestamp now(Timestamp::now());
    if(numEvents > 0)
    {
        LOG_INFO("%d events happened",numEvents);
        fillActiveChannels(numEvents,activeChannels);
        if(static_cast<size_t>(numEvents) == events_.size())
        {
            events_.resize(events_.size()*2);
        }
    }
    else if(numEvents == 0)
    {
        LOG_INFO("nothing happened");
    }
    else
    {
        if(savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_ERROR("EPollPoller::poll()");
        }
    }
    return now;
}

void EPollPoller::fillActiveChannels(int numEvents,ChannelList* activeChannels) const
{
    for(int i = 0;i < numEvents;++i)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void EPollPoller::updateChannel(Channel* channel)
{
    const int index = channel->index();
    LOG_INFO("fd = %d events = %d index = %d",channel->fd(),channel->events(),index);
    if(index == kNew || index == kDeleted)
    {
        int fd = channel->fd();
        if(index == kNew)
        {
            channels_[fd] = channel;
        }
        else
        {
        }

        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD,channel);
    }
    else
    {
        int fd = channel->fd();
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL,channel);
            channel->set_index(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD,channel);
        }
    }
}

void EPollPoller::removeChannel(Channel* channel)
{
    int fd = channel->fd();
    LOG_INFO("fd = %d",fd);
    int index = channel->index();
    size_t n = channels_.erase(fd);

    if(index == kAdded)
    {
        update(EPOLL_CTL_DEL,channel);
    }
    channel->set_index(kNew);
}

void EPollPoller::update(int operation,Channel* channel)
{
    struct epoll_event event;
    ::bzero(&event,sizeof event);
    event.events = channel->events();
    event.data.ptr = channel;
    int fd = channel->fd();
    if(::epoll_ctl(epollfd_,operation,fd,&event) < 0)
    {
        if(operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR("epoll_ctl op = EPOLL_CTL_DEL fd = %d",fd);
        }
        else
        {
            LOG_FATAL("epoll_ctl op = EPOLL_CTL_ADD/EPOLL_CTL_MOD fd = %d",fd);
        }
    }
}