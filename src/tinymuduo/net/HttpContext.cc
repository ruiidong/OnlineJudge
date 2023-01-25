#include "tinymuduo/net/HttpContext.h"
#include "tinymuduo/net/Buffer.h"

bool HttpContext::parseRequest(Buffer* buf, Timestamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    while(hasMore)
    {
        if(state_ == kExpectRequestLine)
        {
            const char* crlf = buf->findCRLF();
            if(crlf)
            {
                ok = processRequestLine(buf->peek(), crlf);
                if(ok)
                {
                    request_.setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    state_ = kExpectHeaders;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if(state_ == kExpectHeaders)
        {
            const char* crlf = buf->findCRLF();
            if(crlf)
            {
                const char* colon = std::find(buf->peek(), crlf, ':');
                if(colon != crlf)
                {
                    request_.addHeader(buf->peek(), colon, crlf);
                }
                else
                {
                    if(request_.method()==HttpRequest::kGet)
                    {
                        state_ = kGotAll;
                        hasMore = false;
                    }
                    else if(request_.method()==HttpRequest::kPost)
                    {
                        state_ = kExpectBody;
                    }
                }
                buf->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if(state_ == kExpectBody)
        {
            // FIXME:
            int value = stoi(request_.getHeader("Content-Length"));
            while(value > 0)
            {
                const char* ands = buf->findAND();
                if(ands)
                {
                    const char* equal = std::find(buf->peek(), ands, '=');
                    request_.addData(buf->peek(), equal, ands);
                    value -= (ands - buf->peek() + 1);
                    buf->retrieveUntil(ands+1);
                }
                else
                {
                    const char* equal = std::find(buf->peek(), buf->peek()+value, '=');
                    request_.addData(buf->peek(), equal, buf->peek()+value);
                    buf->retrieve(value);
                    value = 0;
                }
            }
            state_ = kGotAll;
            hasMore = false;
        }
    }
    return ok;
}

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    if(space != end && request_.setMethod(start, space))
    {
        start = space + 1;
        space = std::find(start, end, ' ');
        if(space != end)
        {
            const char* question = std::find(start, space, '?');
            if(question != space)
            {
                request_.setPath(start, question);
                request_.setQuery(question,space);
            }
            else
            {
                request_.setPath(start, space);
            }
            start = space + 1;
            succeed = end-start==8 && std::equal(start, end-1, "HTTP/1.");
            if(succeed)
            {
                if(*(end-1) == '1')
                {
                    request_.setVersion(HttpRequest::kHttp11);
                }
                else if(*(end-1) == '0')
                {
                    request_.setVersion(HttpRequest::kHttp10);
                }
                else
                {
                    succeed = false;
                }
            } 
        }
    }
    return succeed;
}