#ifndef GKO_SUIT_LOG_H
#define GKO_SUIT_LOG_H

#include <boost/log/trivial.hpp>

#define BLOG(severity) BOOST_LOG_TRIVIAL(severity) \
<< "[" << __FUNCTION__ << ":" << __LINE__ << "] "

#ifndef NDEBUG
    #define DLOG(severity) BLOG(severity)
#else
    #define DLOG(severity) true ? (void) 0 : BLOG(severity)
#endif

namespace bbts {

bool blogInit(const std::string& configDir, const std::string& logDir);

}

#endif //GKO_SUIT_LOG_H
