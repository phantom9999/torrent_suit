#ifndef GKO_SUIT_UNIX_SOCKET_SERVER_WITH_THREAD_H
#define GKO_SUIT_UNIX_SOCKET_SERVER_WITH_THREAD_H


#include "unix_socket_server.h"

namespace bbts {

class UnixSocketServerWithThread : public boost::noncopyable {
public:
    UnixSocketServerWithThread() : _server(_io_service) {}

    ~UnixSocketServerWithThread() {}

    bool start(mode_t mode);

    void join();

    void stop() {
        _server.close();
        _io_service.stop();
    }

    boost::asio::io_service& get_io_service() {
        return _io_service;
    }

    void set_endpoint(const UnixSocketConnection::EndPoint &endpoint) {
        _server.set_endpoint(endpoint);
    }

    void set_accept_callback(UnixSocketServer::AcceptedCallback accept_callback) {
        _server.set_accept_callback(accept_callback);
    }

    void set_read_callback(UnixSocketConnection::RWCallback read_callback) {
        _server.set_read_callback(read_callback);
    }

    void set_write_callback(UnixSocketConnection::RWCallback write_callback) {
        _server.set_write_callback(write_callback);
    }

    void set_close_callback(UnixSocketConnection::CloseCallback close_callback) {
        _server.set_close_callback(close_callback);
    }

    void set_heartbeat_recv_cycle(int cycle) {
        _server.set_heartbeat_recv_cycle(cycle);
    }

    void set_heartbeat_send_cycle(int cycle) {
        _server.set_heartbeat_send_cycle(cycle);
    }

private:
    void run();

    boost::asio::io_service _io_service;
    UnixSocketServer _server;
    boost::thread _thread;
};



}


#endif //GKO_SUIT_UNIX_SOCKET_SERVER_WITH_THREAD_H
