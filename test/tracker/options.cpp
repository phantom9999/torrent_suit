#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>


BOOST_AUTO_TEST_SUITE(s_options)

/**
 * DEFINE_string(dir, "../conf", "conf file dir");
 * DEFINE_string(file, "tracker.conf", "conf file");
 * DEFINE_string(log, "../log/tracker.log", "log file name");
 * DEFINE_string(redis_file, "redis.conf", "redis conf file");
 */
BOOST_AUTO_TEST_CASE(t_options) {
    char* argv[9] = {
        "demo",
        "-dir", "../conf",
        "--file", "tracker.conf",
        "--log", "../log/tracker.log",
        "--redis_file", "redis.conf"
    };
    const int argc = 9;
    using std::string;
    using boost::program_options::options_description;
    using boost::program_options::store;
    using boost::program_options::parse_command_line;
    using boost::program_options::notify;
    using boost::program_options::variables_map;
    using boost::program_options::value;

    string dirname;
    string filename;
    string logfile;
    string redisfile;

    options_description optionsDescription("demo");
    optionsDescription.add_options()
        ("help", "print help")
        ("dir", value<string>(&dirname)->default_value("../conf"), "the path of config")
        ("file", value<string>(&filename)->default_value("tracker.conf"), "the file of config")
        ("log", value<string>(&logfile)->default_value("../log/tracker.log"), "the path of log")
        ("redis_file", value<string>(&redisfile)->default_value("redis.conf"), "the path of redis config")
        ;
    variables_map variablesMap;
    try {
        store(parse_command_line(argc, argv, optionsDescription), variablesMap);
        notify(variablesMap);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }


    if (variablesMap.count("help") != 0) {
        std::cout << optionsDescription << std::endl;
    }



}


BOOST_AUTO_TEST_SUITE_END()

