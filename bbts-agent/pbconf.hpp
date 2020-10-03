#ifndef  BBTS_AGENT_PBCONF_H
#define  BBTS_AGENT_PBCONF_H

#include <cerrno>
#include <fcntl.h>

#include <string>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include "bbts-agent/log.h"

namespace bbts {

inline bool load_pbconf(const std::string &conf_file, google::protobuf::Message *conf) {
    int conf_fd = open(conf_file.c_str(), O_RDONLY);
    if (conf_fd < 0) {
        DEBUG_LOG("open conf {} failed: {}", conf_file.c_str(), errno);
        return false;
    }

    google::protobuf::io::FileInputStream ifs(conf_fd);
    ifs.SetCloseOnDelete(true);
    if (!google::protobuf::TextFormat::Parse(&ifs, conf)) {
        DEBUG_LOG("parse conf {} failed", conf_file.c_str());
        return false;
    }
    return true;
}




}  // namespace bbts

#endif  // BBTS_AGENT_PBCONF_H
