#ifndef ARGUS_COMMON_PATH_H_
#define ARGUS_COMMON_PATH_H_

#include <string>
#include <cstring>

namespace argus {
namespace common {
namespace Path {

inline bool isSeparator(char ch) {
    return ch == '/';
}

std::string getBaseName(const char *filepath) {
    size_t len = strlen(filepath);
    int i = static_cast<int>(len - 1);
    for (; i >= 0; i--) {
        if (isSeparator(filepath[i]))
            break;
    }
    return std::string(filepath + i + 1, filepath + len);
}

std::string getBaseName(const std::string &filepath) {
    return getBaseName(filepath.c_str());
}

std::string getDirectory(const char *filepath) {
    size_t len = strlen(filepath);
    int i = static_cast<int>(len - 1);
    for (; i >= 0; i--) {
        if (isSeparator(filepath[i]))
            break;
    }
    if (i >= 0)
        return std::string(filepath, filepath + i + 1);
    return "";
}

std::string getDirectory(const std::string &filepath) {
    return getDirectory(filepath.c_str());
}

} // namespace Path
} // namespace common
} // namespace argus

#endif  // ARGUS_COMMON_PATH_H_

