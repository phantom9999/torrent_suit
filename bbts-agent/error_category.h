#ifndef BBTS_AGENT_ERROR_CATEGORY_H
#define BBTS_AGENT_ERROR_CATEGORY_H

#include <boost/system/error_code.hpp>

#ifndef BOOST_SYSTEM_NOEXCEPT
    #define BOOST_SYSTEM_NOEXCEPT BOOST_NOEXCEPT
#endif


namespace bbts {

namespace errors {
/* error code */
enum error_code_t {
    NO_ERROR = 0,
    MEM_ALLOC_ERROR,
    MSG_READ_ERROR,
    PROTOBUF_PARSE_ERROR,
    MSG_ARGS_ERROR,
    TORRENT_FILE_ERROR,
    INFOHASH_NOT_CORRECT,
    DUP_TASK_ERROR,
    GENERATE_TASK_ID_FAILED,
    ADD_TORRENT_ERROR,
    TASK_NOT_FOUND,
    TRACKER_ERROR,
    READ_CRED_FAILED,
    CHECK_CRED_FAILED,
    DB_ERROR,
    NO_METADATA,
    PARSE_METADATA_FAILED,
    URI_AGENT_SET_OPTION_ERROR,

    FILE_TOO_LARGE,
    FILE_URI_INVALID,
    FILE_SYSTEM_PROTOCOL_NOT_SUPPORT,
    CLUSTER_NOT_SUPPORT,
    CLUSTER_CONNECT_FAIL,
    CLUSTER_READ_FILE_FAIL,
    MAX_ERROR_NUM,
};

}

/**
 * @brief
 */
class ErrorCategory : public boost::system::error_category {
public:
    ErrorCategory() {}
    virtual ~ErrorCategory() {}
    virtual const char* name() const BOOST_SYSTEM_NOEXCEPT {
        return _s_name;
    }

    virtual std::string message(int ev) const;

private:
    static const char *_s_name;
};

const boost::system::error_category& get_error_category();

}  // namespace bbts
#endif // BBTS_AGENT_ERROR_CATEGORY_H
