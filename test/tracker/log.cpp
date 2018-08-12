#include <boost/test/unit_test.hpp>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>


#include "common/com_log.h"



BOOST_AUTO_TEST_CASE(t_log) {
    /*
    bbts::loadLoggingConfig("../conf/log.conf");
    LOG_WARN() << "yyyyy";
     */
    using std::string;

    log4cpp::PatternLayout* stderr_layout = new log4cpp::PatternLayout();
    stderr_layout->setConversionPattern("[%p][%d{%Y-%m-%d %H:%M:%S}][%t]%m%n");
    log4cpp::OstreamAppender* stderr_appender = new log4cpp::OstreamAppender(
        "stderr_appender",
        &std::cerr);
    stderr_appender->setLayout(stderr_layout);
    log4cpp::Category& category = log4cpp::Category::getRoot();
    category.setAdditivity(false);
    category.setAppender(stderr_appender);
    category.setPriority(log4cpp::Priority::DEBUG);
    LOG_WARN() << "test";

    bbts::LogInstance logInstance("../conf/log.conf");
    LOG_WARN() << "warn";
}




