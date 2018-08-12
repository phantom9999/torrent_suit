#include <boost/test/unit_test.hpp>


#include "common/com_log.h"



BOOST_AUTO_TEST_CASE(t_log) {
    /*
    bbts::loadLoggingConfig("../conf/log.conf");
    LOG_WARN() << "yyyyy";
     */
    bbts::LogInstance logInstance("../conf/log.conf");
    LOG_WARN() << "warn";
}




