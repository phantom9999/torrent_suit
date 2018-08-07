#include "minihttpd/process_info.h"
#include <algorithm>
#include <assert.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h> // snprintf
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>

namespace argus {
namespace common {
namespace internal {

__thread int t_numOpenedFiles = 0;

/**
 *
 * @param d
 * @return
 */
int fdDirFilter(const struct dirent *d) {
    if (::isdigit(d->d_name[0])) {
        ++t_numOpenedFiles;
    }
    return 0;
}

__thread std::vector<pid_t> *t_pids = NULL;

/**
 *
 * @param d
 * @return
 */
int taskDirFilter(const struct dirent *d) {
    if (::isdigit(d->d_name[0])) {
        t_pids->push_back(atoi(d->d_name));
    }
    return 0;
}

/**
 *
 * @param dirpath
 * @param filter
 * @return
 */
int scanDir(const char *dirpath, int (*filter)(const struct dirent *)) {
    struct dirent **namelist = NULL;
    int result = ::scandir(dirpath, &namelist, filter, alphasort);
    assert(namelist == NULL);
    return result;
}

/**
 * 获得格式化好的时间
 * @return
 */
std::string getTimeStr() {
    //30 bytes is enough to hold "[year-month-day : hour-minute-second]"
    char time[30] = {0};
    const char *format = "[%Y-%m-%d %H:%M:%S]";

    struct timeval tv;
    struct tm ltime;
    time_t curtime;
    gettimeofday(&tv, NULL);
    curtime = tv.tv_sec;
    ///Format time
    strftime(time, 30, format, localtime_r(&curtime, &ltime));
    return time;
}

static std::string s_startTime = getTimeStr();

} // namespace internal
} // namespace common
} // namespace argus

using namespace argus;
using namespace argus::common;
using namespace argus::common::internal;

pid_t ProcessInfo::pid() {
    return ::getpid();
}


uid_t ProcessInfo::uid() {
    return ::getuid();
}

std::string ProcessInfo::uidString() {
    char buf[32];
    snprintf(buf, sizeof buf, "%d", uid());
    return buf;
}

std::string ProcessInfo::username() {
    struct passwd pwd;
    struct passwd *result = NULL;
    char buf[8192];
    const char *name = "unknownuser";

    getpwuid_r(uid(), &pwd, buf, sizeof buf, &result);
    if (result) {
        name = pwd.pw_name;
    }
    return name;
}

uid_t ProcessInfo::euid() {
    return ::geteuid();
}

std::string ProcessInfo::euidString() {
    char buf[32];
    snprintf(buf, sizeof buf, "%d", euid());
    return buf;
}

std::string ProcessInfo::startTime() {
    return s_startTime;
}

std::string ProcessInfo::hostname() {
    char buf[64] = "unknownhost";
    buf[sizeof(buf) - 1] = '\0';
    ::gethostname(buf, sizeof buf);
    return buf;
}

std::string ProcessInfo::procStatus() {
    std::string result;
    FILE *fp = fopen("/proc/self/status", "r");
    if (fp) {
        while (!feof(fp)) {
            char buf[8192];
            size_t n = fread(buf, 1, sizeof buf, fp);
            result.append(buf, n);
        }
        fclose(fp);
    }
    return result;
}

/**
 *
 * @return
 */
int ProcessInfo::openedFiles() {
    t_numOpenedFiles = 0;
    scanDir("/proc/self/fd", fdDirFilter);
    return t_numOpenedFiles;
}

std::vector<pid_t> ProcessInfo::threads() {
    std::vector<pid_t> result;
    t_pids = &result;
    scanDir("/proc/self/task", taskDirFilter);
    t_pids = NULL;
    std::sort(result.begin(), result.end());
    return result;
}

std::string ProcessInfo::binaryPath() {
    char path[PATH_MAX] = {0};
    ssize_t length = readlink("/proc/self/exe", path, sizeof(path));
    if (length > 0)
        return std::string(path, length);

    return "<unknown binary path>";
}

