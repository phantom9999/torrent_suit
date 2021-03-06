#ifndef BBTS_AGENT_CONFIG_H
#define BBTS_AGENT_CONFIG_H
#include <string>

#ifndef GINGKO_VERSION
#define GINGKO_VERSION "unknow version"
#endif

namespace bbts {

#if defined(__DATE__) && defined(__TIME__)
static const char BUILD_DATE[] = __DATE__ " " __TIME__;
#else
static const char BUILD_DATE[] = "unknown"
#endif

enum {
    REQ_ADD_TASK = 1,
    REQ_TASK_GETOPT,
    REQ_TASK_SETOPT,
    REQ_AGENT_GETOPT,
    REQ_AGENT_SETOPT,
    REQ_BATCH_CTRL,
    REQ_STATUS,

    REQ_ADD_METADATA = 100,

    RES_BASE = 10000,
    RES_TASK,
    RES_TASK_STATUS,
    RES_BATCH_CTRL,
    RES_BATCH_LIST,
    RES_TASK_GETOPT,
    RES_AGENT_GETOPT,
    RES_STATUS,
};

bool get_user_conf_file(int argc, char* argv[], std::string *conf_file);





} // namespace bbts

#endif // BBTS_AGENT_CONFIG_H
