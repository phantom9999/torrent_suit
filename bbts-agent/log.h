#ifndef BBTS_AGENT_LOG_H
#define BBTS_AGENT_LOG_H

#include "common/com_log.h"

#define OPEN_LOG_R()
#define CLOSE_LOG_R()
#define CLOSE_LOG()

namespace bbts {

enum LogLevel {
    LOG_LEVEL_QUITE   = 0,
    LOG_LEVEL_FATAL   = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_NOTICE  = 3,
    LOG_LEVEL_TRACE   = 4,
    LOG_LEVEL_DEBUG   = 5
};
}

#endif // BBTS_AGENT_LOG_H
