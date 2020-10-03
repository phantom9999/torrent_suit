#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;


BOOST_AUTO_TEST_CASE(t_trivial) {
    BOOST_LOG_TRIVIAL(trace) << "a trace";
    BOOST_LOG_TRIVIAL(debug) << "a debug";
    BOOST_LOG_TRIVIAL(info) << "a info";
    BOOST_LOG_TRIVIAL(warning) << "a warn";
    BOOST_LOG_TRIVIAL(error) << "a error";
    BOOST_LOG_TRIVIAL(fatal) << "a fatal";
}


BOOST_AUTO_TEST_CASE(t_trivial_filter) {

    logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );

    BOOST_LOG_TRIVIAL(trace) << "a trace";
    BOOST_LOG_TRIVIAL(debug) << "a debug";
    BOOST_LOG_TRIVIAL(info) << "a info";
    BOOST_LOG_TRIVIAL(warning) << "a warn";
    BOOST_LOG_TRIVIAL(error) << "a error";
    BOOST_LOG_TRIVIAL(fatal) << "a fatal";


}


BOOST_AUTO_TEST_CASE(t_trivial_file) {
    logging::add_file_log(
        keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                    keywords::format = "[%TimeStamp%]: %Message%"
    );

    logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );

    BOOST_LOG_TRIVIAL(trace) << "a trace";
    BOOST_LOG_TRIVIAL(debug) << "a debug";
    BOOST_LOG_TRIVIAL(info) << "a info";
    BOOST_LOG_TRIVIAL(warning) << "a warn";
    BOOST_LOG_TRIVIAL(error) << "a error";
    BOOST_LOG_TRIVIAL(fatal) << "a fatal";


    // BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::logger_mt)

}






