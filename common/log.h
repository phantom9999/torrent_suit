#ifndef GKO_SUIT_LOG_H
#define GKO_SUIT_LOG_H

#include <fmt/core.h>
#include <boost/log/trivial.hpp>

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

#define BLOG(severity) BOOST_LOG_TRIVIAL(severity) \
<< "[" << __FILENAME__ << ":" << __LINE__ << "] "

#define FATAL_LOG(logFormat, ...) \
do { \
  BOOST_LOG_TRIVIAL(fatal) << fmt::format("[{}:{}] " logFormat, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define ERROR_LOG(logFormat, ...) \
do { \
  BOOST_LOG_TRIVIAL(error) << fmt::format("[{}:{}] " logFormat, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define WARNING_LOG(logFormat, ...) \
do { \
  BOOST_LOG_TRIVIAL(warning) << fmt::format("[{}:{}] " logFormat, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define NOTICE_LOG(logFormat, ...) \
do { \
  BOOST_LOG_TRIVIAL(info) << fmt::format("[{}:{}] " logFormat, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define TRACE_LOG(logFormat, ...) \
do { \
  BOOST_LOG_TRIVIAL(trace) << fmt::format("[{}:{}] " logFormat, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define DEBUG_LOG(logFormat, ...) \
do { \
  BOOST_LOG_TRIVIAL(debug) << fmt::format("[{}:{}] " logFormat, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)

namespace bbts {

bool blogInit(const std::string& configDir, const std::string& logDir);

}

#endif //GKO_SUIT_LOG_H
