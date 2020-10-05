#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

struct GenericOptions {
    bool debug_;
};

struct LsCommand : public GenericOptions {
    bool hidden_;
    std::string path_;
};

struct ChmodCommand : public GenericOptions {
    bool recurse_;
    std::string perms_;
    std::string path_;
};

typedef boost::variant<LsCommand, ChmodCommand> Command;

Command ParseOptions(int argc, const char *argv[])
{
    namespace po = boost::program_options;

    po::options_description global("Global options");
    global.add_options()
        ("debug", "Turn on debug output")
        ("command", po::value<std::string>(), "command to execute")
        ("subargs", po::value<std::vector<std::string> >(), "Arguments for command");

    po::positional_options_description pos;
    pos.add("command", 1).
        add("subargs", -1);

    po::variables_map vm;

    po::parsed_options parsed = po::command_line_parser(argc, argv).
        options(global).
        positional(pos).
        allow_unregistered().
        run();

    po::store(parsed, vm);

    std::string cmd = vm["command"].as<std::string>();

    if (cmd == "ls")
    {
        // ls command has the following options:
        po::options_description ls_desc("ls options");
        ls_desc.add_options()
            ("hidden", "Show hidden files")
            ("path", po::value<std::string>(), "Path to list");

        // Collect all the unrecognized options from the first pass. This will include the
        // (positional) command name, so we need to erase that.
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());

        // Parse again...
        po::store(po::command_line_parser(opts).options(ls_desc).run(), vm);

        LsCommand ls;
        ls.debug_ = vm.count("debug");
        ls.hidden_ = vm.count("hidden");
        ls.path_ = vm["path"].as<std::string>();

        return ls;
    }
    else if (cmd == "chmod")
    {
        // Something similar
    }

    // unrecognised command
    throw po::invalid_option_value(cmd);
}



BOOST_AUTO_TEST_SUITE(s_options)

/**
 * DEFINE_string(dir, "../conf", "conf file dir");
 * DEFINE_string(file, "tracker.conf", "conf file");
 * DEFINE_string(log, "../log/tracker.log", "log file name");
 * DEFINE_string(redis_file, "tracker_redis.conf", "redis conf file");
 */
BOOST_AUTO_TEST_CASE(t_options) {
    char* argv[9] = {
        "demo",
        "-dir", "../conf",
        "--file", "tracker.conf",
        "--log", "../log/tracker.log",
        "--redis_file", "tracker_redis.conf"
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
        ("redis_file", value<string>(&redisfile)->default_value("tracker_redis.conf"), "the path of redis config")
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





BOOST_AUTO_TEST_CASE(NoCommand)
{
    const int argc = 2;
    const char *argv[argc] = { "0", "nocommand" };

    BOOST_CHECK_THROW(
        ParseOptions(argc, argv),
        boost::program_options::invalid_option_value);
}

BOOST_AUTO_TEST_CASE(LsTest)
{
    const int argc = 5;
    const char *argv[argc] = { "0", "--debug", "ls", "--hidden", "--path=./" };

    Command c = ParseOptions(argc, argv);

    BOOST_REQUIRE(boost::get<LsCommand>(&c));
    const LsCommand& ls = boost::get<LsCommand>(c);
    BOOST_CHECK(ls.debug_);
    BOOST_CHECK(ls.hidden_);
    BOOST_CHECK_EQUAL(ls.path_, "./");
}


BOOST_AUTO_TEST_SUITE_END()

