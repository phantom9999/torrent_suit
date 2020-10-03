#ifndef GKO_SUIT_COM_LOG_H
#define GKO_SUIT_COM_LOG_H

#include <string>
#include <boost/noncopyable.hpp>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <fmt/core.h>
#include <boost/log/trivial.hpp>

#define FILEINFO() .operator<<("[")\
.operator<<(__FILE__)\
.operator<<(":")\
.operator<<(__func__)\
.operator<<(":")\
.operator<<(__LINE__)\
.operator<<("] ")


#define LOG_ROOT() log4cpp::Category::getRoot()
#define LOG_DEBUG() LOG4CPP_DEBUG_S(LOG_ROOT()) FILEINFO()
#define LOG_INFO() LOG4CPP_INFO_S(LOG_ROOT()) FILEINFO()
#define LOG_NOTICE() LOG4CPP_NOTICE_S(LOG_ROOT()) FILEINFO()
#define LOG_WARN() LOG4CPP_INFO_S(LOG_ROOT()) FILEINFO()
#define LOG_ERROR() LOG4CPP_ERROR_S(LOG_ROOT()) FILEINFO()
#define LOG_CRIT() LOG4CPP_CRIT_S(LOG_ROOT()) FILEINFO()
#define LOG_ALERT() LOG4CPP_ALERT_S(LOG_ROOT()) FILEINFO()
#define LOG_FATAL() LOG4CPP_FATAL_S(LOG_ROOT()) FILEINFO()
#define LOG_EMERG() LOG4CPP_EMERG_S(LOG_ROOT()) FILEINFO()

#ifndef NDEBUG

#define DLOG_DEBUG() LOG_DEBUG()
#define DLOG_INFO() LOG_INFO()
#define DLOG_NOTICE() LOG_NOTICE()
#define DLOG_WARN() LOG_WARN()
#define DLOG_ERROR() LOG_ERROR()
#define DLOG_CRIT() LOG_CRIT()
#define DLOG_ALERT() LOG_ALERT()
#define DLOG_FATAL() LOG_FATAL()
#define DLOG_EMERG() LOG_EMERG()

#else

#define DLOG_DEBUG() true ? (void) 0 : LOG_DEBUG()
#define DLOG_INFO() true ? (void) 0 : LOG_INFO()
#define DLOG_NOTICE() true ? (void) 0 : LOG_NOTICE()
#define DLOG_WARN() true ? (void) 0 : LOG_WARN()
#define DLOG_ERROR() true ? (void) 0 : LOG_ERROR()
#define DLOG_CRIT() true ? (void) 0 : LOG_CRIT()
#define DLOG_ALERT() true ? (void) 0 : LOG_ALERT()
#define DLOG_FATAL() true ? (void) 0 : LOG_FATAL()
#define DLOG_EMERG() true ? (void) 0 : LOG_EMERG()

#endif




#define LOGLINE(x) LOGLINE_(x)
#define LOGLINE_(x) #x
#define LOGFILEINFO() "[" __FILE__ ":" LOGLINE(__LINE__) "]"

#define FATAL_LOG(logFormat, arg...) \
do { \
  BOOST_LOG_TRIVIAL(fatal) << fmt::format("[{}:{}] " logFormat, __FILE__, __LINE__, ##arg); \
} while (0)

#define WARNING_LOG(logFormat, arg...) \
do { \
  BOOST_LOG_TRIVIAL(warning) << fmt::format("[{}:{}] " logFormat, __FILE__, __LINE__, ##arg); \
} while (0)

#define NOTICE_LOG(logFormat, arg...) \
do { \
  BOOST_LOG_TRIVIAL(info) << fmt::format("[{}:{}] " logFormat, __FILE__, __LINE__, ##arg); \
} while (0)

#define TRACE_LOG(logFormat, arg...) \
do { \
  BOOST_LOG_TRIVIAL(trace) << fmt::format("[{}:{}] " logFormat, __FILE__, __LINE__, ##arg); \
} while (0)

#define DEBUG_LOG(logFormat, arg...) \
do { \
  BOOST_LOG_TRIVIAL(debug) << fmt::format("[{}:{}] " logFormat, __FILE__, __LINE__, ##arg); \
} while (0)


namespace bbts {

bool loadLoggingConfig(const std::string &conf);
bool loggingShutDown();

class LogInstance : boost::noncopyable {
public:
    LogInstance();
    explicit LogInstance(const std::string &conf);
    bool loadConfig(const std::string &conf);
    bool loadDefaultConfig();
    ~LogInstance();
};


}



#endif //GKO_SUIT_COM_LOG_H
