#ifndef BBTS_AGENT_UNIX_SOCKET_SERVER_H
#define BBTS_AGENT_UNIX_SOCKET_SERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread/thread.hpp>

#include "unix_socket_connection.h"


namespace bbts {
static void empty_callback() {}
/**
 * @brief
 */
class UnixSocketServer : public boost::noncopyable {
public:
    typedef boost::asio::local::stream_protocol::socket Socket;
    typedef boost::asio::local::stream_protocol::acceptor Acceptor;
    typedef boost::function<void(const boost::shared_ptr<UnixSocketConnection> &)> AcceptedCallback;

public:
    /**
     *
     * @param io_service
     */
    explicit UnixSocketServer(boost::asio::io_service &io_service);

    /**
     *
     */
    ~UnixSocketServer();

public:
    /**
     *
     * @param mode
     * @return
     */
    bool serve(mode_t mode);

    /**
     * 关闭连接
     */
    void close();

    /**
     *
     * @return
     */
    boost::asio::io_service& get_io_service() {
        return _io_service;
    }

public:
    void set_endpoint(const UnixSocketConnection::EndPoint &endpoint) {
        _endpoint = endpoint;
    }

    void set_accept_callback(AcceptedCallback accept_callback) {
        _accept_callback = accept_callback;
    }

    void set_read_callback(UnixSocketConnection::RWCallback read_callback) {
        _read_callback = read_callback;
    }

    void set_write_callback(UnixSocketConnection::RWCallback write_callback) {
        _write_callback = write_callback;
    }

    void set_close_callback(UnixSocketConnection::CloseCallback close_callback) {
        _close_callback = close_callback;
    }

    void set_heartbeat_recv_cycle(int cycle) {
        _heartbeat_recv_cycle = cycle;
    }

    void set_heartbeat_send_cycle(int cycle) {
        _heartbeat_send_cycle = cycle;
    }

private:
    /**
     * 异步接收请求
     */
    void async_accept();

    /**
     * 建立连接
     * @return
     */
    bool can_connect();

    /**
     * 进行accept
     * @param conn
     * @param ec
     */
    void handle_accepted(boost::shared_ptr<UnixSocketConnection> conn,
            const boost::system::error_code& ec);

private:
    UnixSocketConnection::EndPoint _endpoint;
    boost::asio::io_service &_io_service;
    Acceptor _acceptor;
    int _heartbeat_recv_cycle{0};
    int _heartbeat_send_cycle{0};
    AcceptedCallback _accept_callback{boost::bind(&empty_callback)};
    UnixSocketConnection::RWCallback _read_callback{boost::bind(&empty_callback)};
    UnixSocketConnection::RWCallback _write_callback{boost::bind(&empty_callback)};
    UnixSocketConnection::CloseCallback _close_callback{boost::bind(&empty_callback)};
};



}  // namespace bbts
#endif // BBTS_AGENT_UNIX_SOCKET_SERVER_H
