#ifndef BBTS_AGENT_STRING_UTIL_H
#define BBTS_AGENT_STRING_UTIL_H

#include <cstdint>

#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace bbts {

class StringUtil {
public:

    template <typename T>
    static std::string to_string(T num) {
        std::stringstream strm;
        strm << num;
        return strm.str();
    }

    static std::string bytes_to_readable(int64_t bytes);

    static int64_t readable_to_bytes(const std::string &bytes_string, int64_t unit);
    static inline int64_t readable_to_bytes(const std::string &bytes_string) {
        return readable_to_bytes(bytes_string, 1024LL * 1024LL);
    }

    static void slipt(
            const std::string &str,
            const std::string &delimiter,
            std::vector<std::string> *v);

private:
    StringUtil() = delete;
    ~StringUtil() = delete;
};

}  // namespace bbts
#endif // BBTS_AGENT_STRING_UTIL_H
