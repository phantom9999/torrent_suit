#ifndef BBTS_AGENT_UNIX_SOCKET_CONNECTION_H
#define BBTS_AGENT_UNIX_SOCKET_CONNECTION_H

#include <queue>
#include <utility>
#include <vector>

#include <boost/any.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <google/protobuf/message.h>

#include "header.h"

// struct ucred;

namespace bbts {

/**
 * @brief
 */
class UnixSocketConnection :
        public boost::enable_shared_from_this<UnixSocketConnection>,
        public boost::noncopyable {
public:
    enum MessageType {
        /**
         * heartbeat
         */
        HEARTBEAT,
        /**
         * userdata
         */
        USERDATA,
    };

    typedef boost::asio::local::stream_protocol::socket Socket;
    typedef boost::asio::local::stream_protocol::endpoint EndPoint;
    typedef std::pair<MessageType, boost::shared_ptr<const std::vector<char> > > Message;
    typedef boost::function<void(const EndPoint &)> CloseCallback;
    typedef boost::function<void(const boost::shared_ptr<UnixSocketConnection> &,
            const boost::shared_ptr<const std::vector<char> > &)> RWCallback;

public:
    /**
     *
     */
    ~UnixSocketConnection();

    /**
     *
     * @param io_service
     * @param read_callback
     * @param write_callback
     * @param close_callback
     * @param heartbeat_recv_cycle
     * @param heartbeat_send_cycle
     * @return
     */
    static boost::shared_ptr<UnixSocketConnection> create(
            boost::asio::io_service &io_service,
            RWCallback read_callback,
            RWCallback write_callback,
            CloseCallback close_callback,
            int heartbeat_recv_cycle,
            int heartbeat_send_cycle) {
        return boost::shared_ptr<UnixSocketConnection>(
                new UnixSocketConnection(
                        io_service,
                        read_callback,
                        write_callback,
                        close_callback,
                        heartbeat_recv_cycle,
                        heartbeat_send_cycle));
    }

public:
    /**
     *
     * @return
     */
    Socket& get_socket() {
        return _socket;
    }

    /**
     *
     * @return
     */
    EndPoint& get_remote_endpoint() {
        return _remote_endpoint;
    }

    /**
     *
     */
    void start();

    /**
     *
     * @param data
     */
    void write(const boost::shared_ptr<const std::vector<char> > &data);

    /**
     *
     */
    void close() {
        boost::system::error_code ec;
        _heartbeat_recv_timer.cancel(ec);
        _heartbead_send_timer.cancel(ec);
        _socket.close(ec);
    }

    /**
     *
     * @param user_argument
     */
    void set_user_argument(const boost::any &user_argument) {
        _user_argument = user_argument;
    }

    /**
     *
     * @return
     */
    const boost::any& get_user_argument() const {
        return _user_argument;
    }

private:
    /**
     *
     * @param io_service
     * @param read_callback
     * @param write_callback
     * @param close_callback_
     * @param heartbeat_recv_cycle
     * @param heartbeat_send_cycle
     */
    UnixSocketConnection(
            boost::asio::io_service &io_service,
            RWCallback read_callback,
            RWCallback write_callback,
            CloseCallback close_callback_,
            int heartbeat_recv_cycle,
            int heartbeat_send_cycle);

    /**
     *
     * @param data
     * @param ec
     * @param bytes_readed
     */
    void handle_read_data(
            boost::shared_ptr<std::vector<char> > data,
            const boost::system::error_code &ec,
            size_t bytes_readed);

    /**
     *
     * @param message
     */
    void write_message(const Message &message);
    /**
     *
     * @param ec
     * @param bytes_transferred
     */
    void handle_read_head(const boost::system::error_code &ec, size_t bytes_transferred);
    /**
     *
     * @param ec
     * @param bytes_transferred
     */
    void handle_write_message(const boost::system::error_code &ec, size_t bytes_transferred);
    /**
     *
     * @param type
     * @param data
     */
    void async_write(MessageType type, boost::shared_ptr<const std::vector<char> > data);
    /**
     *
     * @param ec
     */
    void handle_heartbeat_recv(const boost::system::error_code &ec);
    /**
     *
     * @param ec
     */
    void handle_heartbeat_send(const boost::system::error_code &ec);
    /**
     *
     */
    void update_heartbeat_recv_timer();
    /**
     *
     */
    void update_heartbeat_send_timer();

private:
    boost::asio::io_service &_io_service;
    Socket _socket;
    EndPoint _remote_endpoint;
    RWCallback _read_callback;
    RWCallback _write_callback;
    CloseCallback _close_callback;
    Header _read_header;
    Header _write_header;
    std::queue<Message> _write_message_queue;
    int _heartbeat_recv_cycle;
    int _heartbeat_send_cycle;
    boost::asio::deadline_timer _heartbeat_recv_timer;
    boost::asio::deadline_timer _heartbead_send_timer;

    boost::any _user_argument;
};

/**
 *
 * @param connection
 * @param type
 * @param response
 * @return
 */
bool write_message(
        const boost::shared_ptr<UnixSocketConnection> &connection,
        uint32_t type,
        const google::protobuf::Message &response);

/**
 * @brief 发送认证信息给对端
 *
 * @param fd          已连接的unix sock
 * @return          成功返回0，失败返回-1
 */
bool send_cred(int fd);

/**
 * @brief 从对端接收认证信息
 *
 * @param fd          已连接的unix sock
 * @return          成功返回0，失败返回-1
 */
bool recv_cred(int fd, struct ucred *ucptr);

}  // namespace bbts
#endif // BBTS_AGENT_UNIX_SOCKET_CONNECTION_H
