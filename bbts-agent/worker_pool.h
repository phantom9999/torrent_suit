#ifndef BBTS_AGENT_WORKER_POOL_H
#define BBTS_AGENT_WORKER_POOL_H

#include <string>

#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

namespace bbts {

/**
 * asio的工作线程池
 */
class WorkerPool : public boost::noncopyable {
public:
    /**
     *
     * @param tag
     */
    explicit WorkerPool(const std::string &tag);

    /**
     *
     */
    ~WorkerPool() = default;

    /**
     * 启动线程池
     * @param thread_num
     */
    void start(int thread_num);

    /**
     * 等待停止
     */
    void join_all();

    /**
     * 停止
     */
    void stop() {
        _empty_work.reset();
        _io_service.stop();
    }

public:
    /**
     *
     * @return
     */
    boost::asio::io_service& get_io_service() {
        return _io_service;
    }

    /**
     *
     * @return
     */
    const std::string& get_tag() const {
        return _tag;
    }

private:
    void thread_main(int worker_id);

private:
    boost::asio::io_service _io_service;
    boost::scoped_ptr<boost::asio::io_service::work> _empty_work;
    boost::thread_group _threads;
    int _thread_num;
    std::string _tag;
};

}  // namespace bbts
#endif // BBTS_AGENT_WORKER_POOL_H
