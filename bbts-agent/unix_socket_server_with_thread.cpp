#include "unix_socket_server_with_thread.h"

#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include "common/log.h"


namespace bbts {


bool UnixSocketServerWithThread::start(mode_t mode) {
    if (!_server.serve(mode)) {
        return false;
    }
    boost::thread tmp_thread(boost::bind(&UnixSocketServerWithThread::run, this));
    _thread.swap(tmp_thread);
    return true;
}

void UnixSocketServerWithThread::join() {
    _thread.join();
}

void UnixSocketServerWithThread::run() {
    DEBUG_LOG("Unix socket server start success.");
    _io_service.reset();
    _io_service.run();
    DEBUG_LOG("Unix socket server stoped success.");
}


}


