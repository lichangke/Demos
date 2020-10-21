#include <iostream>
#include <log4cplus/log4cplus.h>
#include <log4cplus/initializer.h>

int main() {

    log4cplus::Initializer initializer;
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./config/log.properties"));

}
