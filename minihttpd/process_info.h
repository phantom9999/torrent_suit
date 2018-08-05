#ifndef ARGUS_COMMON_PROCESSINFO_H_
#define ARGUS_COMMON_PROCESSINFO_H_

#include <vector>
#include <string>
#include <algorithm>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/syscall.h>

namespace argus {
namespace common {
namespace ProcessInfo {

pid_t gettid(void);

pid_t pid();
std::string pidString();

uid_t uid();
std::string uidString();
std::string username();

uid_t euid();
std::string euidString();

std::string startTime();
std::string hostname();

/// read /proc/self/status
std::string procStatus();

int openedFiles();
int maxOpenFiles();

int numThreads();
std::vector<pid_t> threads();

std::string binaryPath();
std::string binaryDirectory();
std::string binaryName();

} // namespace ProcessInfo
} // namespace common
} // namespace argus

#endif  // ARGUS_COMMON_PROCESSINFO_H_

