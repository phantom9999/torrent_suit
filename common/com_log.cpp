#include "com_log.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/RollingFileAppender.hh>


namespace bbts {

bool loadLoggingConfig(const std::string &conf) {
    try {
        log4cpp::PropertyConfigurator::configure(conf);
        return true;
    } catch (log4cpp::ConfigureFailure& f) {
        return false;
    }
}

bool loggingShutDown() {
    log4cpp::Category::getRoot().shutdown();
    return true;
}

LogInstance::LogInstance(const std::string &conf) {
    this->loadConfig(conf);
}

LogInstance::~LogInstance() {
    log4cpp::Category::getRoot().shutdown();
}

LogInstance::LogInstance() {
    this->loadDefaultConfig();
}

bool LogInstance::loadConfig(const std::string &conf) {
    try {
        log4cpp::PropertyConfigurator::configure(conf);
        return true;
    } catch (log4cpp::ConfigureFailure& f) {
        std::cerr << f.what() << std::endl;
        return false;
    }
}

bool LogInstance::loadDefaultConfig() {
    typedef boost::system::error_code ErrorCode;
    typedef boost::filesystem::path Path;

    std::string format{"[%p][%d{%Y-%m-%d %H:%M:%S}][%t]%m%n"};
    std::string logdir{"log"};
    std::string filename{"bbts.log"};

    ErrorCode ec;
    Path path = boost::filesystem::canonical("/proc/self/exe", ec);
    if (ec.value() == 0) {
        logdir = path.parent_path().parent_path().string() + "/" + logdir;
    }
    if (!boost::filesystem::exists(logdir, ec)) {
        boost::filesystem::create_directories(logdir, ec);
    }


    std::string logfile = logdir + '/' + filename;
    log4cpp::PatternLayout* file_layout = new log4cpp::PatternLayout();
    log4cpp::PatternLayout* std_layout = new log4cpp::PatternLayout();
    file_layout->setConversionPattern(format);

    log4cpp::Appender* file_appender = new log4cpp::RollingFileAppender(
        logdir, logfile, 10 * 1024 * 1024, 1
    );
    file_appender->setLayout(file_layout);

    log4cpp::OstreamAppender* stderr_appender = new log4cpp::OstreamAppender(
        "stderr_appender",
        &std::cerr);
    stderr_appender->setLayout(std_layout);
    log4cpp::Category& category = log4cpp::Category::getRoot();
    category.setAdditivity(false);
    category.setAppender(stderr_appender);
    category.addAppender(file_appender);
    category.setPriority(log4cpp::Priority::DEBUG);
    return true;
}

}







