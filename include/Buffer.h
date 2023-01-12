#pragma once

#include <vector>
#include <algorithm>


using namespace std;

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)  //explicit用于禁用单参数构造函数的隐式转换
        : buffer_(kCheapPrepend + initialSize)  //这里不仅预留空间，还会构造对象
        , readerIndex_(kCheapPrepend)
        , writerIndex_(kCheapPrepend)
    {
    }

    size_t readableBytes() const    //返回可读数据大小
    {
        return writerIndex_ - readerIndex_;
    }

    size_t writeableBytes() const   //返回可写数据大小
    {
        return buffer_.size() - writerIndex_;
    }

    size_t prependableBytes() const //返回预留空间大小
    {
        return readerIndex_;
    }

    const char* peek() const    //返回可读数据的地址
    {
        return begin() + readerIndex_;
    }

    void retrieve(size_t len)   //当你从缓冲区中读取了数据后，需要挪动readerIndex就调用此函数
    {
        if(len < readableBytes())
        {
            readerIndex_ += len;
        }
        else
        {
            retrieveAll();
        }
    }

    void retrieveAll()
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    void retrieveUntil(const char* end)
    {
        retrieve(end-peek());
    }

    void append(const string& str)    //向缓冲区中写入数据
    {
        append(str.data(),str.size());
    }

    void append(const char* data,size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data,data+len,beginWrite());
        hasWritten(len);
    }

    void ensureWritableBytes(size_t len)    //确保有足够的空间写入数据
    {
        if(writeableBytes() < len)
        {
            makeSpace(len);
        }
    }

    void hasWritten(size_t len) //挪动writeIndex
    {
        writerIndex_ += len;
    }

    char* beginWrite()  //可写数据的位置
    {
        return begin() + writerIndex_;
    }

    const char* beginWrite() const
    {
        return begin() + writerIndex_;
    }

    void unwrite(size_t len)    //挪动writerIndex
    {
        writerIndex_ -= len;
    }

    void prepend(const void* data,size_t len)   //向预留空间写入数据
    {
        readerIndex_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d,d+len,begin()+readerIndex_);
    }

    ssize_t readFd(int fd,int* savedErrno); //读取连接缓冲区中的数据到缓冲区中
    ssize_t writeFd(int fd);
private:
    char* begin()   //返回缓冲区的起始地址
    {
        return &*buffer_.begin();
    }

    const char* begin() const
    {
        return &*buffer_.begin();
    }

    void makeSpace(size_t len)  //扩容
    {
        if(writeableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writerIndex_+len);
        }
        else
        {
            size_t readable = readableBytes();
            std::copy(begin()+readerIndex_,
                      begin()+writerIndex_,
                      begin()+kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
        }
    }

private:
    std::vector<char> buffer_;  //可扩容的缓冲区
    size_t readerIndex_;
    size_t writerIndex_;
};