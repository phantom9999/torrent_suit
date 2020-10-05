#include "tcp_server.h"
#include "disk_manager.h"
// #include "common/log.h"
#include "common/com_log.h"

using boost::system::error_code;
using boost::asio::ip::tcp;

using bbts::TcpServer;
using bbts::DiskManager;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: %s port\n", argv[0]);
        return 1;
    }
    bbts::LogInstance logInstance("conf/log.conf");

    bbts::g_disk_manager->start();
    tcp::endpoint listen_endpoint(tcp::v4(), atoi(argv[1]));
    if (!bbts::g_tcp_server->init(listen_endpoint)) {
        // FATAL_LOG("init server failed");
        LOG_FATAL() << "init server failed";
        return 1;
    }
    bbts::g_tcp_server->set_upload_limit(1000*1024);
    bbts::g_tcp_server->set_max_connection(1000);

    error_code ec;
    bbts::g_tcp_server->serve(ec);
    if (ec) {
        // FATAL_LOG("io service: %s\n", ec.message().c_str());
        LOG_FATAL() << "io service: " << ec.message().c_str();
        return 2;
    }
    bbts::g_disk_manager->stop();
    bbts::g_disk_manager->join();
    return 0;
}

