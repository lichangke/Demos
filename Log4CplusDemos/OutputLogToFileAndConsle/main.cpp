#include <iostream>
#include <log4cplus/log4cplus.h>
#include <log4cplus/initializer.h>

int main() {
    //第1步：用Initializer类进行初始化
    log4cplus::Initializer initializer;
    //第2步：创建ConsoleAppender;
    //SharedAppenderPtr 指向一个Appender的指针,这里new 了一个 ConsoleAppender
    log4cplus::SharedAppenderPtr consoleAppender(new log4cplus::ConsoleAppender());
    // std::ios_base::app 每次进行写入操作的时候都会重新定位到文件的末尾.
    log4cplus::SharedAppenderPtr fileAppender(new log4cplus::FileAppender(LOG4CPLUS_TEXT("./logs/log.txt"),std::ios_base::app,true,true));

    //第3步：设置consoleAppender的名称和输出格式（SimpleLayout） fileAppender的名称和pattern格式（PatternLayout）
    // unique_ptr 智能指针
    consoleAppender->setName(LOG4CPLUS_TEXT("console"));
    consoleAppender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::SimpleLayout));
    fileAppender->setName(LOG4CPLUS_TEXT("file"));
    // 参见 https://log4cplus.sourceforge.io/docs/html/classlog4cplus_1_1PatternLayout.html
    //2020/10/16 16:13:03,481.502 [140068134246208] INFO  test - Hello world [/home/leacock/CLionProjects/Log4CplusDemo/OutputLogToFileAndConsle/main.cpp:33]
    // %D{%Y/%m/%d %H:%M:%S,%Q} 2020/10/16 16:13:03,481.502 时间 %D 表本地时间, 年/月/日/ 时：分：秒：毫秒
    //[%t]  [140068134246208] 输出生成日志事件的线程的线程id
    //%-5p  INFO  输出日志事件的日志级别  %-5 如果名称小于5个字符长，则右侧加空格。
    //%c  test 用于输出记录日志的是哪个logger，这里 在getInstance(LOG4CPLUS_TEXT ("test"))设置了名称
    //%m  Hello world  输出与logger关联的应用程序提供消息。
    //%l 等同于 "%F:%L"   F：用于输出日志请求发出时的文件名 L：用于输出从何处发出日志请求的行号。
    //%n 输出换行
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y/%m/%d %H:%M:%S,%Q} [%t] %-5p %c - %m [%l]%n");
    fileAppender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));

    //第4步：获得一个Logger实例，并设置其日志输出等级阈值
    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT ("test"));
    logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);

    //第5步：为Logger实例添加ConsoleAppender 与 FileAppender
    logger.addAppender(consoleAppender);
    logger.addAppender(fileAppender);

    //第6步：使用宏将日志输出
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Hello world"));


}
