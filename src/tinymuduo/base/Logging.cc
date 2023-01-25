#include "tinymuduo/base/Logging.h"
#include "tinymuduo/base/Timestamp.h"

Logger &Logger::getInstance()
{
    static Logger logger;
    return logger;
}

Logger::LogLevel Logger::logLevel()
{
    return logLevel_;
}

void Logger::setLogLevel(Logger::LogLevel level)
{
    logLevel_ = level;
}

void Logger::log(string msg)
{
    switch (logLevel_)
    {
    case DEBUG:
        cout << "[DEBUG]"; 
        break;
    case INFO:
        cout << "[INFO]";
        break;
    case ERROR:
        cout << "[ERROR]";
        break;
    case FATAL:
        cout << "[FATAL]";
        break;
    default:
        break;
    }
    cout << Timestamp::now().toString() << " : " << msg << std::endl;
}