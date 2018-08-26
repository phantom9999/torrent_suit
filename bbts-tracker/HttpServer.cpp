#include "bbts-tracker/HttpServer.h"
#include <boost/bind.hpp>
#include "common/com_log.h"
#include "minihttpd/eventloop.h"
#include "minihttpd/minihttpd.h"

#include "bbts-tracker/StatusManager.h"

namespace bbts {

using std::string;
using ::argus::common::EventLoop;
using ::argus::common::MiniHttpd;

/**
 * 启动线程,并注册函数
 *
 * todo: 去掉全局变量
 */
bool HttpServer::start(uint16_t port) {
    boost::thread t(boost::bind(&HttpServer::Run, this, port));
    thread_.swap(t);
    return g_pStatusManager
        ->RegisterItem("monitor_status_queries", StatusManager::COUNTING_ITEM);
}


/**
 * 添加回调函数
 *
 *
 */
bool HttpServer::SetCallback(const string &path, callback_fn cb) {
    // detect status
    const timespec unit = {1, 0};
    for (int i = 0; !httpd_ && i < 10; ++i) {
        nanosleep(&unit, NULL);
    }

    if (!httpd_) {
        LOG_WARN() << "must set httpd callback after it started";
        return false;
    }

    string retmsg;
    bool succ = httpd_->setCallBack(path, cb, &retmsg);
    if (!succ) {
        LOG_WARN() << "set minihttpd callback " << path << " fail: "
                     << retmsg;
    } else {
        LOG_INFO() << "set minihttpd callback " << path << " success!";
    }
    return succ;
}

/**
 * 创建时间循环, 并添加回调函数
 *
 *
 */
void HttpServer::Run(uint16_t port) {
    loop_ = new EventLoop();
    httpd_ = new MiniHttpd(loop_, port);
    LOG_INFO() << "Httpd init, port:" << port;
    bool succ =
        httpd_->setCallBack("monitor_status", QueryMonitorStatusCallBack);
    assert(succ);
    LOG_INFO()
        << "Set CallBack(QueryMonitorStatusCallBack) for request monitor_status";
    loop_->loop();
    LOG_INFO() << "Httpd thread exit";

}

/**
 * 获取状态信息, 并进行统计
 */
string HttpServer::QueryMonitorStatusCallBack(const string &query) {
    string result;
    LOG_INFO() << "get query monitor status request";
    g_pStatusManager->ShowAllStatistics(&result);
    g_pStatusManager->IncreaseItemCounting("monitor_status_queries");
    return result;
}

} // namespace bbts
