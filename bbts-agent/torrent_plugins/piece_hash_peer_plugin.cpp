#include "piece_hash_peer_plugin.h"
#include <libtorrent/peer_id.hpp>
#include <libtorrent/io.hpp>
#include <libtorrent/peer_request.hpp>
#include <libtorrent/bt_peer_connection.hpp>


namespace bbts {


bool PieceHashPeerPlugin::write_request(libtorrent::peer_request const &r) {
    using std::vector;
    using libtorrent::detail::write_string;
    using libtorrent::detail::write_uint8;
    using libtorrent::detail::write_uint32;
    using libtorrent::detail::write_uint64;
    if (r.start != 0) {
        return false;
    }

    vector<char> msg_buffer;
    std::back_insert_iterator<vector<char> > ptr(msg_buffer);
    write_uint32(0, ptr);
    write_uint8(libtorrent::bt_peer_connection::msg_extended, ptr);
    write_uint8(MESSAGE_INDEX, ptr);
    write_uint8(MESSAGE_PIECE_HASH_REQUEST, ptr);
    write_uint32(r.piece, ptr);
    char *p = &msg_buffer[0];
    write_uint32(msg_buffer.size() - 4, p);
    _peer_connection.send_buffer(&msg_buffer[0], msg_buffer.size());
    _peer_connection.setup_send();
    return false;
}

void PieceHashPeerPlugin::write_piece_hash(int piece) {
    using std::vector;
    using libtorrent::detail::write_string;
    using libtorrent::detail::write_uint8;
    using libtorrent::detail::write_uint32;
    using libtorrent::detail::write_uint64;
    vector<char> msg_buffer;
    std::back_insert_iterator<vector<char> > ptr(msg_buffer);
    write_uint32(0, ptr);
    write_uint8(libtorrent::bt_peer_connection::msg_extended, ptr);
    write_uint8(MESSAGE_INDEX, ptr);
    write_uint8(MESSAGE_PIECE_HASH_RESPONSE, ptr);
    write_uint32(piece, ptr);
    write_string(_torrent.torrent_file().hash_for_piece(piece).to_string(),
                 ptr);
    char *p = &msg_buffer[0];
    write_uint32(msg_buffer.size() - 4, p);
    _peer_connection.send_buffer(&msg_buffer[0], msg_buffer.size());
    _peer_connection.setup_send();
}

bool PieceHashPeerPlugin::on_extended(int length,
                                      int msg,
                                      libtorrent::buffer::const_interval body) {
    if (MESSAGE_INDEX != msg) {
        return false;
    }

    if (body.left() < 1) {
        return true;
    }
    int type = libtorrent::detail::read_uint8(body.begin);

    switch (type) {
        case MESSAGE_PIECE_HASH_REQUEST: {
            if (body.left() < 4) {
                return true;
            }
            int piece = libtorrent::detail::read_uint32(body.begin);
            write_piece_hash(piece);
            break;
        }

        case MESSAGE_PIECE_HASH_RESPONSE: {// failed
            if (body.left() < 24) {
                return true;
            }
            int piece = libtorrent::detail::read_uint32(body.begin);
            libtorrent::sha1_hash hash(body.begin);
            _torrent.set_piece_hash(piece, hash);
            break;
        }
        default:
            break;
    }
    return true;
}

}



