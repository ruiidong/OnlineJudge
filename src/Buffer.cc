#include "Buffer.h"

#include <sys/uio.h>

ssize_t Buffer::readFd(int fd,int* savedErrno) //读取连接中的数据到缓冲区中
{
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writeableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd,vec,iovcnt);
    if(n < 0)
    {
        *savedErrno = errno;
    }
    else if(n <= writable)
    {
        writerIndex_ += n;
    }
    else
    {
        writerIndex_ = buffer_.size();
        append(extrabuf,n-writable);
    }

    return n;
}