#ifndef ARGUS_COMMON_PROCESSINFO_H_
#define ARGUS_COMMON_PROCESSINFO_H_

#include <vector>
#include <string>
#include <algorithm>
#include <dirent.h>
#include <pwd.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/syscall.h>

namespace argus {
namespace common {
namespace ProcessInfo {

/**
 * 获得进程id
 * @return
 */
pid_t pid();

/**
 * 获得实际用户id
 * @return
 */
uid_t uid();

/**
 * udi转成字符串
 */
std::string uidString();

/**
 * 获得当前用户名
 */
std::string username();

/**
 * 获得有效用户id
 * @return
 */
uid_t euid();
/**
 * 有效用户id转字符串
 * @return
 */
std::string euidString();

/**
 * 获得
 * @return
 */
std::string startTime();
std::string hostname();

/// read /proc/self/status
std::string procStatus();

int openedFiles();

std::vector<pid_t> threads();

std::string binaryPath();


} // namespace ProcessInfo
} // namespace common
} // namespace argus

#endif  // ARGUS_COMMON_PROCESSINFO_H_

