#include "disk_manager.h"

#include <boost/crc.hpp>

#include "tcp_connection.h"
#include "uri_piece_request.h"

#include "bbts-agent/consistent_hash_ring.hpp"
#include "common/LazySingleton.hpp"
#include "common/log.h"
#include "io.hpp"

using boost::shared_ptr;
using boost::system::error_code;

namespace bbts {

DiskManager *g_disk_manager = LazySingleton<DiskManager>::instance();

DiskManager::DiskManager() : _disk_thread_number(kReadThreadNumber) {
    _cache_manager = new CacheManager(_ios);
}

DiskManager::~DiskManager() {
    if (_cache_manager != NULL) {
        delete _cache_manager;
        _cache_manager = NULL;
    }
}

void DiskManager::start() {
    assert(_disk_thread_number > 0);
    // start read disk thread
    for (int i = 0; i < _disk_thread_number; ++i) {
        _thread_group.create_thread(boost::bind(&DiskManager::run, this));
        shared_ptr<IoServiceStrand> strand(new IoServiceStrand(_ios));
        _strand_list.push_back(strand);
    }

}

void DiskManager::run() {
    TRACE_LOG("disk manager thread run begin");
    boost::asio::io_service::work work(_ios);
    boost::system::error_code ec;
    _ios.run(ec);
    if (ec) {
        WARNING_LOG("[disk manager thread run fail: {}", ec.message().c_str());
    }
    TRACE_LOG("disk manager thread run end");
}

void DiskManager::async_piece_request(shared_ptr<TcpConnection> connection,
        const URIPieceRequest &request) {
    int index = get_request_thread_hash(request);
    shared_ptr<IoServiceStrand> strand = _strand_list.at(index);
    _ios.post(strand->wrap(
                boost::bind(&DiskManager::piece_request, this, connection, request)));
}

void DiskManager::piece_request(shared_ptr<TcpConnection> connection,
        const URIPieceRequest &request) {
    std::string error_msg;
    shared_ptr<Buffer> buffer = _cache_manager->fetch(request, error_msg);
    if (!error_msg.empty() || !buffer) {
        std::string msg("fetch block failed:");
        msg.append(error_msg);
        boost::mutex::scoped_lock lock(_mutex);
        connection->write_failed_message(msg);
        // close by remote peer
        // connection->close();
        return;
    }

    {
        boost::mutex::scoped_lock lock(_mutex);
        shared_ptr<Buffer> header(new Buffer(13));
        char *ptr = &(*header)[0];
        write_uint32(9 + buffer->size(), ptr);
        write_uint8(7, ptr);
        write_uint32(request.piece_index, ptr);
        write_uint32(request.piece_offset, ptr);
        connection->write_data(header);
        connection->write_data(buffer);
    }
}

void DiskManager::async_piece_hash_request(shared_ptr<TcpConnection> connection,
        const URIPieceRequest &request) {
    int index = get_request_thread_hash(request);
    shared_ptr<IoServiceStrand> strand = _strand_list.at(index);
    _ios.post(strand->wrap(
                boost::bind(&DiskManager::piece_hash_request, this, connection, request)));
}

void DiskManager::piece_hash_request(shared_ptr<TcpConnection> connection,
        const URIPieceRequest &request) {
    std::string error_msg;
    shared_ptr<Buffer> buffer = _cache_manager->calculate_hash(request, error_msg);
    if (!error_msg.empty() || !buffer) {
        std::string msg("calculate piece hash failed:");
        msg.append(error_msg);
        boost::mutex::scoped_lock lock(_mutex);
        connection->write_failed_message(msg);
        return;
    }

    {
        boost::mutex::scoped_lock lock(_mutex);
        connection->async_write_piece_hash(request.piece_index, buffer);
    }
}

int DiskManager::get_request_thread_hash(const URIPieceRequest &request) {
    boost::crc_32_type crc32Type;
    crc32Type.process_bytes(request.infohash.c_str(), request.infohash.length());
    uint32_t infohash_hash_index = crc32Type();
    return (request.piece_index + infohash_hash_index) % _disk_thread_number;
}

} // namespace bbts
