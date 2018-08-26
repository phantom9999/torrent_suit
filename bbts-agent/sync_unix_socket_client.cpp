#include "sync_unix_socket_client.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/system/error_code.hpp>

#include "bbts-agent/log.h"

namespace bbts {

SyncUnixSocketClient::SyncUnixSocketClient(boost::asio::io_service &io_service) :
    _io_service(io_service),
    _socket(io_service) {}

SyncUnixSocketClient::~SyncUnixSocketClient() {
    close();
}

bool SyncUnixSocketClient::connect(const UnixSocketConnection::EndPoint &endpoint) {
    boost::system::error_code ec;
    _socket.connect(endpoint, ec);
    if (ec) {
        DEBUG_LOG("connect to server(%s) failed: %s",
                  endpoint.path().c_str(), ec.message().c_str());
    }
    return !ec;
}

bool SyncUnixSocketClient::write_data(const boost::shared_ptr<const std::vector<char> > &data) {
    if (!data) {
        return true;
    }
    Header header(UnixSocketConnection::USERDATA, data->size());
    boost::array<boost::asio::const_buffer, 2> buffers = {
        boost::asio::buffer(&header, sizeof(header)),
        boost::asio::buffer(*data)
    };
    boost::system::error_code ec;
    boost::asio::write(_socket, buffers, ec);
    if (ec) {
        DEBUG_LOG("write data to server failed: %s", ec.message().c_str());
    }
    return !ec;
}

bool SyncUnixSocketClient::read_data(boost::shared_ptr<std::vector<char> > *data) {
    assert(data);
    boost::system::error_code ec;
    Header header;
    boost::asio::read(_socket, boost::asio::buffer(&header, sizeof(header)), ec);
    if (ec) {
        DEBUG_LOG("read data from server failed: %s", ec.message().c_str());
        return false;
    }
    if (!header.is_valid()) {
        DEBUG_LOG("valid header failed!");
        return false;
    }
    data->reset(new std::vector<char>(header.length()));
    boost::asio::read(_socket, boost::asio::buffer(*data->get()), ec);
    return !ec;
}

void SyncUnixSocketClient::close() {
    boost::system::error_code ec;
    _socket.close(ec);
}




}


