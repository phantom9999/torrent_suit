#include "source_peer_plugin.h"

#include <libtorrent/torrent_info.hpp>
#include <libtorrent/io.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include "bbts-agent/peer_source_request_failed_alert.h"
#include "bbts-agent/path.h"

namespace bbts {

void SourcePeerPlugin::tick() {
    if (!_tick_once && _torrent.valid_metadata()) {
        _tick_once = true;
        _peer_connection.set_upload_only(true);
        _peer_connection.incoming_have_all();
        _peer_connection.incoming_unchoke();
    }
}

void SourcePeerPlugin::add_handshake(libtorrent::entry &e) {
    const libtorrent::torrent_info &ti = _torrent.torrent_file();
    libtorrent::entry &message = e["m"];
    message["piece_length"] = ti.piece_length();
    message["root_path"] = _source_path;
}

bool SourcePeerPlugin::write_request(libtorrent::peer_request const &r) {
    if (_dynamic_hash_check && r.start == 0) {
        write_request_with_type(MESSAGE_URI_PIECE_HASH_REQUEST, r);
    }

    return write_request_with_type(MESSAGE_URI_PIECE_REQUEST, r);
}

bool SourcePeerPlugin::on_extended(int length,
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
        case MESSAGE_URI_PIECE_FAILED: {// failed
            if (body.left() < length - 1) {
                return true;
            }
            libtorrent::aux::session_impl &impl = _torrent.session();
            if (impl.m_alerts.should_post<PeerSourceRequestFailedAlert>()) {
                impl.m_alerts.post_alert(PeerSourceRequestFailedAlert(
                    _torrent.get_handle(),
                    _peer_connection.remote(),
                    _peer_connection.pid(),
                    std::string(body.begin, body.end)));
            }
            _peer_connection.peer_info_struct()->connectable = false;
            _torrent.remove_peer(&_peer_connection);
            _peer_connection.disconnect(libtorrent::errors::invalid_request);
            break;
        }

        case MESSAGE_URI_PIECE_HASH_RESPONSE: {
            if (body.left() < 24) {
                return true;
            }
            int piece = libtorrent::detail::read_uint32(body.begin);
            libtorrent::sha1_hash hash(body.begin);
            _torrent.set_piece_hash(piece, hash);
            break;
        }

        default:
            _peer_connection.disconnect(libtorrent::errors::invalid_request, 2);
            break;
    }
    return true;
}


bool SourcePeerPlugin::write_request_with_type(int type, libtorrent::peer_request const &r) {
    using std::vector;
    using libtorrent::detail::write_string;
    using libtorrent::detail::write_uint8;
    using libtorrent::detail::write_uint32;
    using libtorrent::detail::write_uint64;
    const libtorrent::torrent_info& ti = _torrent.torrent_file();
    int length = r.length;
    int start = r.start;
    if (type == MESSAGE_URI_PIECE_HASH_REQUEST) {
        length = ti.piece_size(r.piece);
        start = 0;
    }
    vector<libtorrent::file_slice> files = ti.map_block(r.piece, start, length);

    vector<char> msg_buffer;
    std::back_insert_iterator<vector<char> > ptr(msg_buffer);
    write_uint32(0, ptr);
    write_uint8(libtorrent::bt_peer_connection::msg_extended, ptr);
    write_uint8(MESSAGE_INDEX, ptr);
    write_uint8(type, ptr);
    write_uint32(r.piece, ptr);
    write_uint32(start, ptr);
    for (vector<libtorrent::file_slice>::iterator i = files.begin(); i != files.end(); ++i) {
        write_string(Path::subpath(ti.file_at(i->file_index).path), ptr);
        write_uint8('\0', ptr);
        write_uint64(i->offset, ptr);
        write_uint32(i->size, ptr);
    }

    char *p = &msg_buffer[0];
    write_uint32(msg_buffer.size() - 4, p);
    _peer_connection.send_buffer(&msg_buffer[0], msg_buffer.size());
    _peer_connection.setup_send();
    return true;
}


}



