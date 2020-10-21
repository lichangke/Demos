#include <iostream>
#include <iomanip>
#include <log4cplus/log4cplus.h>

int main() {
    log4cplus::Initializer initializer;

    log4cplus::BasicConfigurator config;
    config.configure();

    log4cplus::Logger logger = log4cplus::Logger::getInstance(
            LOG4CPLUS_TEXT("main"));

    LOG4CPLUS_INFO(logger,
                   LOG4CPLUS_TEXT("This is")
                           << LOG4CPLUS_TEXT(" a reall")
                           << LOG4CPLUS_TEXT("y long message.") << std::endl
                           << LOG4CPLUS_TEXT("Just testing it out") << std::endl
                           << LOG4CPLUS_TEXT("What do you think?"));
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a bool: ") << true);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a char: ")
            << LOG4CPLUS_TEXT('x'));
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a short: ")
            << static_cast<short>(-100));
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a unsigned short: ")
            << static_cast<unsigned short>(100));
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a int: ") << 1000);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a unsigned int: ") << 1000U);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a long(hex): ")
            << std::hex << 100000000L);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a unsigned long: ")
            << static_cast<unsigned long>(100000000U));
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a float: ") << 1.2345f);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a double: ")
            << std::setprecision(15)
            << 1.2345234234);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a long double: ")
            << std::setprecision(15)
            << 123452342342.342L);

    return 0;
}
