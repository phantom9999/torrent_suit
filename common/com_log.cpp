#include "com_log.h"
#include <iostream>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Priority.hh>



namespace bbts {

bool loadLoggingConfig(const std::string &conf) {
    try {
        log4cpp::PropertyConfigurator::configure(conf);
        return true;
    } catch (log4cpp::ConfigureFailure& f) {
        return false;
    }
}

bool loggingShutdown() {
    log4cpp::Category::getRoot().shutdown();
    return true;
}

LogInstance::LogInstance(const std::string &conf) {
    try {
        log4cpp::PropertyConfigurator::configure(conf);
    } catch (log4cpp::ConfigureFailure& f) {
        std::cerr << f.what() << std::endl;
    }
}

LogInstance::~LogInstance() {
    log4cpp::Category::getRoot().shutdown();
}

}







