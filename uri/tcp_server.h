#ifndef BBTS_AGENT_TCP_SERVER_H
#define BBTS_AGENT_TCP_SERVER_H

#include <map>
#include <queue>
#include <vector>

#include <boost/asio/deadline_timer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include <boost/unordered_map.hpp>

#include "tcp_connection.h"

namespace bbts {

/**
 * @brief
 */

class TcpServer : public boost::noncopyable {
private:
    struct UploadLimitData {
        // 1048576 means 10M/s, for 10 * 1024 * 1024 * (100 / 1000)
        int64_t set_upload_limit{1048576}; // user set limit
        int64_t upload_limit{1048576};     // real upload limit by agent
        int64_t connection_num{0};         // connection number for this infohash
        int64_t total_wait_bytes{0};       // wait bytes for sending
        int64_t total_upload_per_unit{0};  // per unit upload bytes
    };

    struct TcpConnectionQuota {
        boost::shared_ptr<TcpConnection> conn;
        int64_t require_quota{0};
    };

    typedef boost::unordered_map<int64_t, boost::shared_ptr<TcpConnection> > ConnectionMap;

    typedef boost::unordered_map<std::string, UploadLimitData> UploadLimitMap;
public:
    TcpServer();
    ~TcpServer();

    /**
     * 启动
     * @return
     */
    bool start();

    /**
     * 停止
     */
    void stop();

    /**
     * 等待
     */
    void join() {
        _thread.join();
    }

    /**
     * 设置端口
     * @param port
     */
    void set_tcp_port(int port) {
        Tcp::endpoint listen_endpoint(Tcp::v4(), port);
        _endpoint = listen_endpoint;
    }

    /**
     *
     * @return
     */
    boost::asio::io_service& get_io_service() {
        return _ios;
    }

    /**
     * 删除链接, 如果被引用则引用计数-1
     * @param conn
     * @return
     */
    bool del_connection(boost::shared_ptr<TcpConnection> conn);

    /**
     *
     * @param max_connection
     */
    void set_max_connection(int64_t max_connection) {
        if (max_connection == 0) {
            // 0 means not limit
            max_connection = -1;
        }
        _max_connection_num = max_connection;
    }

    /**
     *
     * @param default_limit
     */
    void set_default_upload_limit_for_data(int default_limit) {
        if (_default_upload_limit >= 1) {
            _default_upload_limit = default_limit;
        }
    }

    /**
     *
     * @param upload_limit
     */
    void set_total_upload_limit(int64_t upload_limit);  // total upload_limit in byte/s unit

    /**
     *
     * @param infohash
     * @param upload_limit
     * @return
     */
    bool set_upload_limit(const std::string &infohash, int64_t upload_limit);

    /**
     *
     * @param infohash
     * @param total_upload
     */
    void add_total_upload_per_unit_by_infohash(const std::string &infohash, int64_t total_upload);

    /**
     *
     * @param infohash
     * @param wait_bytes
     */
    void add_wait_by_infohash(const std::string &infohash, int64_t wait_bytes);

    /**
     *
     * @param infohash
     * @param wait_bytes
     */
    void del_wait_by_infohash(const std::string &infohash, int64_t wait_bytes);

    /**
     *
     * @return
     */
    const time_t current_timestamp() {
        return _current_timestamp;
    }

    /**
     *
     * @param infohash
     */
    void init_infohash_upload_limit(const std::string &infohash);

    /**
     *
     * @param infohash
     */
    void delete_infohash_upload_limit(const std::string &infohash);

    /**
     *
     * @param conn
     * @param require_quota
     */
    void add_connection_to_wait(boost::shared_ptr<TcpConnection> conn, int64_t require_quota);



private:
    /**
     *
     * @return
     */
    bool init();

    /**
     *
     */
    void async_accept();

    /**
     *
     * @param conn
     * @param ec
     */
    void accept_cb(boost::shared_ptr<TcpConnection> conn,
            const boost::system::error_code& ec);

    /**
     *
     * @param conn
     */
    void add_connection(boost::shared_ptr<TcpConnection> conn);

    /**
     *
     */
    void speed_timer_callback();

    /**
     *
     */
    void adjust_upload_limit();

    /**
     *
     */
    void calculate_connection_quota();

    /**
     *
     */
    void on_tick();

    /**
     *
     */
    void run();

    /**
     *
     * @return
     */
    bool is_reached_max_connection() {
        if (_max_connection_num <= 0) {
            return false;
        }

        return _connection_map.size() + 1 > static_cast<size_t>(_max_connection_num);
    }

private:

    static const int kCheckPeriod = 100;  // check per 100ms for speed limit
    static const int kDefaultUploadLimit = 10;  // default 10M/s

    Tcp::endpoint _endpoint;
    boost::asio::io_service _ios;
    boost::thread _thread;
    Tcp::acceptor _acceptor;
    int64_t _max_connection_num;
    int _default_upload_limit;
    boost::asio::deadline_timer _speed_timer;
    boost::asio::deadline_timer _tick_timer;
    int64_t _total_upload_limit;      // in byte unit, upload limit in per kCheckPeriod
    UploadLimitMap _upload_limit_map;
    // connection wait for quota queue 
    std::queue<TcpConnectionQuota> _connection_quota_queue;
    // all connection map, each connection is identified by an unique id
    ConnectionMap _connection_map;
    int64_t _inner_connection_id;
    time_t _current_timestamp;
};

extern TcpServer *g_tcp_server;

}  // namespace bbts

#endif // BBTS_AGENT_TCP_SERVER_H
