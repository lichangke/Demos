#include <iostream>
#include <log4cplus/log4cplus.h>

int main() {
    // 1 初始化
    log4cplus::Initializer initializer;
    // 2 创建 appender
    log4cplus::SharedAppenderPtr rollingFileAppender(new log4cplus::RollingFileAppender(LOG4CPLUS_TEXT("./logs/roll_log/roll_file_log"), 200*1024, 5,true,true) );
//    // 3 设置consoleAppender的名称和输出格式,这里使用缺省的 TTCCLayout 格式
//    rollingFileAppender->setName(LOG4CPLUS_TEXT("roll file test"));
//    rollingFileAppender->setLayout(std::unique_ptr<log4cplus::Layout>(new  log4cplus::TTCCLayout()));

    rollingFileAppender->setName(LOG4CPLUS_TEXT("roll file test"));
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y/%m/%d %H:%M:%S,%Q} [%t] %-5p %c - %m [%l]%n");
    rollingFileAppender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));

    // 4 获得一个Logger实例，并设置其日志输出等级阈值
    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("roll_file"));
    logger.setLogLevel(log4cplus::TRACE_LOG_LEVEL);

    // 5 为Logger实例添加RollingFileAppender
    logger.addAppender(rollingFileAppender);

    for ( int  i = 0 ; i < 5000 ;i ++ )     {
        LOG4CPLUS_DEBUG(logger, "Entering loop #" << i);
    }
    return 0;
}
