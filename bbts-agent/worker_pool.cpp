#include "bbts-agent/worker_pool.h"

#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "bbts-agent/log.h"

namespace bbts {

WorkerPool::WorkerPool(const std::string &tag) : _thread_num(0), _tag(tag) {}


void WorkerPool::thread_main(int worker_id) {
    TRACE_LOG("[{}][{}] run begin.", _tag.c_str(), worker_id);
    boost::system::error_code ec;
    _io_service.run(ec);
    if (ec) {
        WARNING_LOG("[{}]thread pool run fail: {}", _tag.c_str(), ec.message().c_str());
    }
    TRACE_LOG("[{}][{}] run end.", _tag.c_str(), worker_id);
}

void WorkerPool::start(int thread_num) {
    _empty_work.reset(new boost::asio::io_service::work(_io_service));
    _thread_num = thread_num;
    _io_service.reset();
    for (int i = 0; i < _thread_num; ++i) {
        _threads.create_thread(boost::bind(&WorkerPool::thread_main, this, i));
    }
}

void WorkerPool::join_all() {
    _threads.join_all();
}

}  // namespace bbts
