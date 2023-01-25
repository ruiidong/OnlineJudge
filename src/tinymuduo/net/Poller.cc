#include "tinymuduo/net/Poller.h"
#include "tinymuduo/net/Channel.h"

Poller::Poller(EventLoop* loop)
    : ownerLoop_(loop)
{
}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const
{
    ChannelMap::const_iterator it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}