#include <iostream>
#include <log4cplus/log4cplus.h>

int main() {
    // Initialization and deinitialization.
    // 初始化
    log4cplus::Initializer initializer;

    // 这两句 使用ConsoleAppender 和 simple layout 来配置root logger
    log4cplus::BasicConfigurator config;
    config.configure();
    // 获取 名为 main 的 logger 句柄
    log4cplus::Logger logger = log4cplus::Logger::getInstance(
            LOG4CPLUS_TEXT("main"));
    // 输出日志信息
    LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Hello, World! WARN"));
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Hello, World! INFO"));
    return 0;
}
