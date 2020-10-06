

#ifndef GKO_SUIT_SYNC_UNIX_SOCKET_CLIENT_H
#define GKO_SUIT_SYNC_UNIX_SOCKET_CLIENT_H

#include <boost/asio/io_service.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "unix_socket_connection.h"

namespace bbts {


class SyncUnixSocketClient : public boost::noncopyable {
public:
    explicit SyncUnixSocketClient(boost::asio::io_service &ios);
    ~SyncUnixSocketClient();
    bool connect(const UnixSocketConnection::EndPoint &endpoint);
    bool write_data(const boost::shared_ptr<const std::vector<char> > &data);
    bool read_data(boost::shared_ptr<std::vector<char> > *data);
    void close();

    UnixSocketConnection::Socket& get_socket() {
        return _socket;
    }

private:
    boost::asio::io_service &_io_service;
    UnixSocketConnection::Socket _socket;
};

}


#endif //GKO_SUIT_SYNC_UNIX_SOCKET_CLIENT_H
