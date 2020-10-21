#include <iostream>
#include <log4cplus/log4cplus.h>
#include <log4cplus/initializer.h>
int main() {
    //第1步：用Initializer类进行初始化
    log4cplus::Initializer initializer;

    //第2步：创建ConsoleAppender;
    //SharedAppenderPtr 指向一个Appender的指针,这里new 了一个 ConsoleAppender
    log4cplus::SharedAppenderPtr appender(new log4cplus::ConsoleAppender());

    //第3步：设置Appender的名称和输出格式（SimpleLayout）
    // unique_ptr 智能指针
    appender->setName(LOG4CPLUS_TEXT("console"));
    appender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::SimpleLayout));

    //第4步：获得一个Logger实例，并设置其日志输出等级阈值
    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT ("test"));
    logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);

    //第5步：为Logger实例添加ConsoleAppender
    logger.addAppender(appender);

    //第6步：使用宏将日志输出
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Hello world"));

    return 0;
}
