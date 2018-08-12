#include <boost/test/unit_test.hpp>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <boost/filesystem.hpp>

#include "common/com_log.h"



BOOST_AUTO_TEST_CASE(t_log) {
    /*
    bbts::loadLoggingConfig("../conf/log.conf");
    LOG_WARN() << "yyyyy";
     */
    using std::string;
    LOG_WARN() << "test";

    bbts::LogInstance logInstance;
    LOG_WARN() << "warn";

    boost::system::error_code ec;
    boost::filesystem::path path = boost::filesystem::canonical("/proc/self/exe", ec);
    LOG_INFO() << path.string();


    logInstance.loadConfig("../conf/log.conf");
    LOG_INFO() << path.parent_path();



}




