#include <csignal>

#include <sstream>

#include <boost/asio/ip/host_name.hpp>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/Thrift.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include "bbts-tracker/HttpServer.h"
#include "bbts-tracker/KeyTypeRWLock.hpp"
#include "bbts-tracker/RedisManager.h"
#include "bbts-tracker/StatusManager.h"
#include "tracker-protocol/Announce.h"
#include "bbts-tracker/tracker/AnnounceHandler.h"
#include "bbts-tracker/tracker/CommonConf.h"
#include "bbts-tracker/tracker/InfoHashGarbageCleaner.h"
#include "bbts-tracker/tracker/PeerHandler.h"
#include "bbts-tracker/tracker/RemotePeersSyncronizer.h"
#include "pb_config/redis_conf.pb.h"
#include "pb_config/tracker_conf.pb.h"

#include <common/log.h>


using std::string;
using boost::bind;
using boost::scoped_ptr;
using boost::shared_ptr;
using boost::system::error_code;
using boost::thread;
using boost::thread_group;
using apache::thrift::concurrency::ThreadFactory;
using apache::thrift::concurrency::ThreadManager;
using apache::thrift::concurrency::PosixThreadFactory;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::server::TNonblockingServer;
using apache::thrift::transport::TTransportFactory;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::TException;
using apache::thrift::TProcessor;

using namespace bbts;
using namespace bbts::tracker;

#ifdef BBTS_TRACKER_VERSION

static const char VERSIONID[] = BBTS_TRACKER_VERSION;

#else
static const char VERSIONID[] = "unknown";
#endif

TNonblockingServer *g_my_server;

PeerHandler *peer_handler = nullptr;

/**
 *
 * @param query
 * @return
 */
static string ForceQuit(const string &query) {
    g_my_server->stop();
    return "have send stop singal\n";
}

/**
 *
 * @param query
 * @return
 */
static string ShowInfohashs(const string &query) {
    return peer_handler->ShowInfohashs(query);
}

/**
 *
 * @param query
 * @return
 */
static string ShowPeers(const string &query) {
    return peer_handler->ShowPeers(query);
}

/**
 *
 * @param port
 * @return
 */
static string GenerateTrackerId(int port) {
    string hostname;
    error_code ec;
    hostname = boost::asio::ip::host_name(ec);
    if (ec) {
        BLOG(warning) << "get host name failed";
        hostname = "unknow";
    } else {
        string::size_type pos = hostname.rfind(".baidu.com");
        hostname = hostname.substr(0, pos);
    }
    DLOG(info) << "hostname is:" << hostname;
    return (boost::format("%s:%d") % hostname % port).str();
}

/**
 *
 * @param update_cycle_second
 */
static void StartStatusItems(int32_t update_cycle_second) {
    g_pStatusManager->RegisterItem("request", StatusManager::COUNTING_ITEM);
    g_pStatusManager
        ->RegisterItem("valid_request", StatusManager::COUNTING_ITEM);
    g_pStatusManager
        ->RegisterItem("redis_to_write_num_master", StatusManager::NUM_ITEM);
    g_pStatusManager
        ->RegisterItem("redis_to_write_num_slave", StatusManager::NUM_ITEM);
    g_pStatusManager
        ->RegisterItem("redis_to_syncronize_num", StatusManager::NUM_ITEM);
    g_pStatusManager->Start(update_cycle_second);
}

/**
 *
 * @param http_server
 * @param httpd_port
 */
static void StartHttpServer(HttpServer *http_server, int httpd_port) {
    http_server->start(httpd_port);
    http_server->SetCallback("infohashs", &ShowInfohashs);
    http_server->SetCallback("peers", &ShowPeers);
    http_server->SetCallback("forcequit", &ForceQuit);
}

/**
 * 初始化日志配置
 * @param log_path
 */
 /*
inline static void InitLogging(bbts::tracker::TrackerLog &trackerLog) {
    google::InitGoogleLogging("tracker");
    //FLAGS_logtostderr = 1;
    google::SetLogDestination(google::INFO, (trackerLog.log_dir() + "/info_").c_str());
    google::SetLogDestination(google::WARNING, (trackerLog.log_dir() + "/warning_").c_str());
    google::SetLogDestination(google::ERROR, (trackerLog.log_dir() + "/error_").c_str());
    google::SetLogDestination(google::FATAL, (trackerLog.log_dir() + "/fatal_").c_str());
    google::SetStderrLogging(google::GLOG_INFO);
}*/

/**
 * 设置信号调用
 */
inline static void SetSingalProcess() {
    auto processTerminate = [](int sigNum)->void {
        g_my_server->stop();
    };
    signal(SIGINT, processTerminate);
    signal(SIGTERM, processTerminate);
    signal(SIGQUIT, processTerminate);
    signal(SIGHUP, processTerminate);
    signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char **argv) {
    using std::string;
    using OptionsDescription = boost::program_options::options_description;
    using VariablesMap = boost::program_options::variables_map;
    using boost::program_options::store;
    using boost::program_options::parse_command_line;
    using boost::program_options::notify;
    using boost::program_options::value;

    // 命令行解析
    string dirname;
    string filename;

    OptionsDescription optionsDescription("tracker");
    optionsDescription.add_options()
        ("help", "print help")
        ("dir", value<string>(&dirname)->default_value("conf"), "the path of config")
        ("config", value<string>(&filename)->default_value("tracker.conf"), "the file of config")
        ;
    VariablesMap variablesMap;
    try {
        store(parse_command_line(argc, argv, optionsDescription), variablesMap);
        notify(variablesMap);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    if (variablesMap.count("help") != 0) {
        std::cout << optionsDescription << std::endl;
        return 0;
    }

    // 配置文件解析
    TrackerConf tracker_conf;
    if (!LoadConf(dirname + "/" + filename, &tracker_conf)) {
        std::cout << "parse tracker config error" << std::endl;
        return 1;
    }

    // bbts::LogInstance logInstance(dirname + "/" + tracker_conf.log_conf());
    bbts::logInit();

    RedisConf redisConf;
    if (!LoadConf(dirname + "/" + tracker_conf.redis_conf(), &redisConf)) {
        BLOG(error) << "parse redis config error";
        return 1;
    }

    PeerInfo::set_tracker_id(GenerateTrackerId(tracker_conf.port()));

    shared_ptr<InfoHashMap> local_map(new InfoHashMap());
    shared_ptr<InfoHashMap> remote_map(new InfoHashMap());
    shared_ptr<InfoHashQueue> queue_to_syncronize(new InfoHashQueue());
    assert(local_map && remote_map && queue_to_syncronize);
    InfoHashGarbageCleaner garbage_cleaner;
    garbage_cleaner.Initialize(tracker_conf.info_hash_expire_time(),
                               local_map,
                               remote_map);

    StartStatusItems(tracker_conf.monitor_cycle_second());
    HttpServer http_server;
    StartHttpServer(&http_server, tracker_conf.httpd_port());

    if (!g_pRedisManager->Start(redisConf)) {
        BLOG(error) << "can't initialize redis";
        return 1;
    }

    thread garbage_cleaner_thread(bind(
        &InfoHashGarbageCleaner::ThreadFunc, &garbage_cleaner
    ));
    thread_group remote_peers_syncronizers;
    for (int i = 0; i < tracker_conf.remote_peers_syncronizer_num(); ++i) {
        remote_peers_syncronizers.create_thread(
            bind(RemotePeersSyncronizer::ThreadFunc,
                 queue_to_syncronize,
                 remote_map));
    }
    BLOG(info) << "remote_peers_syncronizers started";

    shared_ptr<AnnounceHandler> handler(new AnnounceHandler());
    peer_handler = &handler->get_peer_handler();
    peer_handler->SetInfoHashMaps(local_map,
                                  remote_map,
                                  queue_to_syncronize,
                                  tracker_conf.info_hash_expire_time());
    shared_ptr<TProcessor> processor(new AnnounceProcessor(handler));
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    shared_ptr<ThreadManager> threadManager =
        ThreadManager::newSimpleThreadManager(tracker_conf.thread_num());
    shared_ptr<ThreadFactory> threadFactory(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    g_my_server = new TNonblockingServer(processor,
                                         protocolFactory,
                                         tracker_conf.port(),
                                         threadManager);
    SetSingalProcess();

    try {
        threadManager->start();
        g_my_server->serve();
    }
    catch (std::exception &e) {
        BLOG(error) << "catch exception: " << e.what();
        return 9;
    }
    return 0;
}
