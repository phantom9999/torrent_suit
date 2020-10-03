#include "log.h"

#include <string>
#include <fstream>
#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>

#include <boost/filesystem.hpp>

#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>

namespace BLogging = boost::log;
namespace BSrc = BLogging::sources;
namespace BTri = BLogging::trivial;
namespace BFileSystem = boost::filesystem;

namespace bbts {

bool blogInit(const std::string& configDir, const std::string& logDir) {
    typedef BFileSystem::path Path;
    typedef boost::system::error_code ErrorCode;

    Path workspace = BFileSystem::current_path();
    BLOG(trace) << "use workspace: " << workspace.string();
    Path confPath = workspace / configDir;
    Path logPath = workspace / logDir;

    BLogging::add_common_attributes();
    BLogging::register_simple_formatter_factory<BTri::severity_level, char>(
        "Severity");
    BLogging::register_simple_filter_factory<BTri::severity_level, char>(
        "Severity");

    if (!BFileSystem::exists(logPath)) {
        BLOG(trace) << "mkdir " << logPath.string();
        BFileSystem::create_directory(logPath);
    }

    try {
        std::ifstream file(confPath.string());
        BLogging::init_from_stream(file);
    }
    catch (const std::exception &e) {
        BLOG(error) << "init_logger is fail, read log config file fail. curse: "
                  << e.what() << " use " << confPath.string() << std::endl;
        return false;
    }
    return true;
}

}