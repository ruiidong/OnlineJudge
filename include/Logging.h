#pragma once
#include <string>
#include <iostream>
#include <unistd.h>

#include "noncopyable.h"

using namespace std;

//#define MDEBUG

#define MDEBUG

#ifdef MDEBUG
#define LOG_DEBUG(logFormat,...) \
        do \
        { \
            char buf[1024] = {0}; \
            snprintf(buf,sizeof buf,logFormat,##__VA_ARGS__); \
            auto& logger = Logger::getInstance(); \
            logger.setLogLevel(Logger::DEBUG); \
            logger.log(buf); \
        } while (0)
#else
#define LOG_DEBUG(logFormat,...) 
#endif

#define LOG_INFO(logFormat,...) \
        do \
        { \
            char buf[1024] = {0}; \
            snprintf(buf,sizeof buf,logFormat,##__VA_ARGS__); \
            auto& logger = Logger::getInstance(); \
            logger.setLogLevel(Logger::INFO); \
            logger.log(buf); \
        } while (0)

#define LOG_ERROR(logFormat,...) \
        do \
        { \
            char buf[1024] = {0}; \
            snprintf(buf,sizeof buf,logFormat,##__VA_ARGS__); \
            auto& logger = Logger::getInstance(); \
            logger.setLogLevel(Logger::ERROR); \
            logger.log(buf); \
        } while (0)

#define LOG_FATAL(logFormat,...) \
        do \
        { \
            char buf[1024] = {0}; \
            snprintf(buf,sizeof buf,logFormat,##__VA_ARGS__); \
            auto& logger = Logger::getInstance(); \
            logger.setLogLevel(Logger::FATAL); \
            logger.log(buf); \
            exit(0); \
        } while (0)

class Logger : noncopyable
{
public:
    enum LogLevel //日志等级
    {
        DEBUG, //调试
        INFO,  //信息
        ERROR, //错误
        FATAL, //致命，程序退出
    };

    static Logger &getInstance();
    LogLevel logLevel();
    void setLogLevel(LogLevel level);
    void log(string msg);
private:
    Logger(){}
    LogLevel logLevel_;
};
