#include "minihttpd/process_inspector.h"
#include "minihttpd/process_info.h"
#include "minihttpd/minihttpd.h"
#include <cstdio>
#include "common/com_log.h"

namespace argus {
namespace common {

void ProcessInspector::registerCallBacks(MiniHttpd *httpd) {
    std::string retmsg;
    if (!httpd->setCallBack(MiniHttpd::baseInfoPath,
                            ProcessInspector::baseInfo,
                            &retmsg)) {
        LOG_ERROR() << "failed registerCallBacks" << retmsg;
    }
    httpd->setCallBack("start_time", ProcessInspector::startTime);
    httpd->setCallBack("account", ProcessInspector::account);
    httpd->setCallBack("binary_path", ProcessInspector::binaryPath);
    httpd->setCallBack("proc/pid", ProcessInspector::pid);
    httpd->setCallBack("proc/status", ProcessInspector::procStatus);
    httpd->setCallBack("proc/opened_files", ProcessInspector::openedFiles);
    httpd->setCallBack("proc/threads", ProcessInspector::threads);
}

std::string ProcessInspector::baseInfo(const std::string &) {
    std::string content;
    char buf[32];

    // binaryPath
    content += "binary:" + ProcessInspector::binaryPath(content);

    // startTime
    content += "startTime:" + ProcessInspector::startTime(content);

    // startAccount
    content += ProcessInspector::account(content);

    // pid
    snprintf(buf, sizeof buf, "pid:%d\n", ProcessInfo::pid());
    content += buf;

    // openedFiles
    snprintf(buf, sizeof buf, "openedFiles:%d\n", ProcessInfo::openedFiles());
    content += buf;

    // proc/status
    content += ProcessInfo::procStatus();

    return content;
}

std::string ProcessInspector::pid(const std::string &) {
    char buf[32];
    snprintf(buf, sizeof buf, "%d\n", ProcessInfo::pid());
    return buf;
}

std::string ProcessInspector::procStatus(const std::string &) {
    return ProcessInfo::procStatus();
}

std::string ProcessInspector::openedFiles(const std::string &) {
    char buf[32];
    snprintf(buf, sizeof buf, "%d\n", ProcessInfo::openedFiles());
    return buf;
}

std::string ProcessInspector::threads(const std::string &) {
    std::vector<pid_t> threads = ProcessInfo::threads();
    std::string result;
    for (size_t i = 0; i < threads.size(); ++i) {
        char buf[32];
        snprintf(buf, sizeof buf, "%d\n", threads[i]);
        result += buf;
    }
    return result;
}

std::string ProcessInspector::startTime(const std::string &) {
    return ProcessInfo::startTime() + "\n";
}

std::string ProcessInspector::account(const std::string &) {
    std::string result = "uid:";
    result += ProcessInfo::uidString();
    result += "\n";

    result += "euid:";
    result += ProcessInfo::euidString();
    result += "\n";

    result += "username:";
    result += ProcessInfo::username();
    result += "\n";
    return result;
}

std::string ProcessInspector::binaryPath(const std::string &) {
    std::string path = ProcessInfo::binaryPath();
    return path + "\n";
}

} // namespace common
} // namespace argus

