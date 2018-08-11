
#include <boost/test/unit_test.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>


BOOST_AUTO_TEST_CASE(datetime) {
    using std::stringstream;
    using std::cout;
    using std::endl;
    using boost::posix_time::ptime;
    using boost::posix_time::second_clock;
    using boost::posix_time::time_facet;

    ptime now = second_clock::local_time();
    stringstream stringBuffer;
    static std::locale loc(stringBuffer.getloc(), new time_facet("[%Y-%m-%d %H:%M:%S]"));
    stringBuffer.imbue(loc);
    stringBuffer << now;
    cout << stringBuffer.str() << endl;
    std::string result2 = stringBuffer.str();

    char time[30] = {0};
    const char *format = "[%Y-%m-%d %H:%M:%S]";

    struct timeval tv;
    struct tm ltime;
    time_t curtime;
    gettimeofday(&tv, NULL);
    curtime = tv.tv_sec;

    ///Format time
    strftime(time, 30, format, localtime_r(&curtime, &ltime));
    std::string result1(time);
    boost::trim(result1);
    boost::trim(result2);
    BOOST_CHECK_EQUAL(result2, result1);



}


