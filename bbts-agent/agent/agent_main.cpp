#include <fcntl.h>
#include <getopt.h>
#include <csignal>

#include <string>

#include "bbts-agent/agent/task_manager.h"
#include "bbts-agent/config.h"
#include "bbts-agent/file.h"
#include "common/log.h"
#include "bbts-agent/path.h"
#include "bbts-agent/pbconf.hpp"
#include "bbts-agent/process_info.h"
#include "bbts-agent/timer_util.h"
#include "bbts-agent/config.h"


using std::string;
using bbts::message::AgentConfigure;
using boost::posix_time::hours;

namespace bbts {
namespace agent {

/**
 * 初始化运行目录
 * @return
 */
static bool init_agent_running_path() {
    if (!Path::mkdir(g_agent_configure->working_dir(), 0755)) {
        FATAL_LOG("mkdir {} failed.", g_agent_configure->working_dir().c_str());
        return false;
    }

    if (chdir(g_agent_configure->working_dir().c_str()) != 0) {
        FATAL_LOG("chdir to working path {} failed", g_agent_configure->working_dir().c_str());
        return false;
    }

    if (!Path::mkdir(g_agent_configure->resume_dir(), 0755)) {
        FATAL_LOG("mkdir {} failed", g_agent_configure->resume_dir().c_str());
        return false;
    }

    // for download task stat log file
    mode_t mode = umask(0);
    int fd = open(g_agent_configure->task_stat_file().c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd < 0) {
        FATAL_LOG("open {} failed: {}", g_agent_configure->task_stat_file().c_str(), errno);
        return false;
    }
    close(fd);
    chmod(g_agent_configure->task_stat_file().c_str(), 0666);

    fd = open(g_agent_configure->peer_stat_file().c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd < 0) {
        FATAL_LOG("open {} failed: {}", g_agent_configure->peer_stat_file().c_str(), errno);
        return false;
    }
    close(fd);
    chmod(g_agent_configure->peer_stat_file().c_str(), 0666);

    fd = open(g_agent_configure->download_log_file().c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd < 0) {
        FATAL_LOG("open {} failed: {}", g_agent_configure->download_log_file().c_str(), errno);
        return false;
    }
    close(fd);
    chmod(g_agent_configure->download_log_file().c_str(), 0666);
    umask(mode);
    return true;
}

/**
 * 设置信号捕捉
 */
static void set_signal_action() {
    // 中断/退出 信号
    struct sigaction sa{};
    sa.sa_flags = SA_RESETHAND;
    sa.sa_handler = [](int sig) {
        NOTICE_LOG("catch sigal {}!", sig);
        g_task_manager->stop();
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGQUIT, &sa, nullptr);

    // kill 信号
    struct sigaction act{};
    act.sa_sigaction = [](int sig, siginfo_t * sig_info, void *) {
        NOTICE_LOG("receive terminal signal, sig_num={}, sending_process_pid={}, uid={}, status={}",
                   sig, sig_info->si_pid, sig_info->si_uid, sig_info->si_status);

        time_t now_time = time(nullptr);
        string cmdline = ProcessInfo::get_process_cmdline_by_pid(sig_info->si_pid);
        string exe = ProcessInfo::get_link_info_by_pid_and_type(sig_info->si_pid, "exe");
        string cwd = ProcessInfo::get_link_info_by_pid_and_type(sig_info->si_pid, "cwd");
        NOTICE_LOG("terminal time: {}, process cmdline={}, exe={}, cwd={}",
                   now_time, cmdline.c_str(), exe.c_str(), cwd.c_str());

        g_task_manager->stop();
    };
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGTERM, &act, nullptr);

    // SIGPIPE信号
    struct sigaction sigpipe{};
    sigpipe.sa_flags = 0;
    sigpipe.sa_handler = [](int sig){
        NOTICE_LOG("catch sigal {}!", sig);
    };
    sigemptyset(&sigpipe.sa_mask);
    sigaction(SIGPIPE, &sigpipe, nullptr);
}

/**
 * 检测stat文件
 */
static void check_stat_file() {
    struct stat statbuf{};
    if (::stat(g_agent_configure->task_stat_file().c_str(), &statbuf) == 0) {
        if (statbuf.st_size > 10 * 1024 * 1024) {
            if (truncate(g_agent_configure->task_stat_file().c_str(), 0) != 0) {
                WARNING_LOG("truncate file {} size to 0 failed: {}",
                            g_agent_configure->task_stat_file().c_str(), errno);
            }
        }
    } else {
        WARNING_LOG("can't stat file {}: {}", g_agent_configure->task_stat_file().c_str(), errno);
    }

    if (::stat(g_agent_configure->peer_stat_file().c_str(), &statbuf) == 0) {
        if (statbuf.st_size > 10 * 1024 * 1024) {
            if (truncate(g_agent_configure->peer_stat_file().c_str(), 0) != 0) {
                WARNING_LOG("truncate file {} size to 0 failed: {}",
                            g_agent_configure->peer_stat_file().c_str(), errno);
            }
        }
    } else {
        WARNING_LOG("can't stat file {}: {}", g_agent_configure->peer_stat_file().c_str(), errno);
    }
}



} // namespace detail
} // namespace bbts




int main(int argc, char* argv[]) {
    using namespace bbts;
    using namespace bbts::agent;

    // 读取参数
    string conf_file(g_process_info->root_dir() + "/conf/agent.conf");
    string user_conf;
    if (bbts::get_user_conf_file(argc, argv, &user_conf)) {
        conf_file.assign(user_conf);
    }

    // 读取配置
    if (!load_pbconf(conf_file, g_agent_configure)) {
        return 1;
    }

    // 初始化日志
    if (!bbts::blogInit("conf/agent_log.conf", "log")) {
        return 1;
    }

    NOTICE_LOG("bbts agent version: {}", GINGKO_VERSION);

    // 初始化路径
    if (!init_agent_running_path()) {
        return 1;
    }

    // 配置锁定
    boost::system::error_code ec;
    if (!File::lock(g_agent_configure->lock_file(), ec)) {
        FATAL_LOG("lock file {} failed: [{}]{}", g_agent_configure->lock_file().c_str(),
                  ec.value(), ec.message().c_str());
        return 1;
    }

    // 设置信号
    set_signal_action();

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (!g_task_manager->start()) {
        return 1;
    }

    boost::asio::deadline_timer timer(g_task_manager->get_io_service());
    timer_run_cycle("check stat file", timer, hours(1), boost::bind(&check_stat_file));
    g_task_manager->join();
    // 删除由protobuf产生的对象
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}


