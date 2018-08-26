#include "torrent_plugin.h"

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/peer_info.hpp>
#include <libtorrent/torrent.hpp>

#include "bbts-agent/path.h"
#include "bbts-agent/process_info.h"
#include "torrent_stat_plugin.h"
#include "torrent_source_plugin.h"
#include "torrent_piece_hash_plugin.h"

using std::map;
using std::pair;
using std::string;
using std::vector;

using boost::shared_ptr;
using boost::system::error_code;
using boost::weak_ptr;
using libtorrent::aux::session_impl;
using libtorrent::file_slice;
using libtorrent::peer_connection;
using libtorrent::peer_plugin;
using libtorrent::peer_request;
using libtorrent::tcp;
using libtorrent::torrent;
using libtorrent::torrent_info;
using libtorrent::torrent_plugin;
using libtorrent::detail::write_string;
using libtorrent::detail::write_uint8;
using libtorrent::detail::write_uint32;
using libtorrent::detail::write_uint64;

namespace bbts {

void LibtorrentPlugin::on_tick() {
    if (++_tick_count % _release_file_interval == 0) {
        shared_ptr<session_impl> impl = _impl.lock();
        if (impl) {
            impl->release_files();
        }
    }
}

shared_ptr<torrent_plugin> create_torrent_stat_plugin(torrent *t, void *args) {
    return shared_ptr<torrent_plugin>(new TorrentStatPlugin(*t));
}

shared_ptr<torrent_plugin> create_no_check_torrent_source_plugin(torrent *t, void *args) {
    map<tcp::endpoint, string> *source_peers = (map<tcp::endpoint, string> *)args;
    return shared_ptr<torrent_plugin>(new TorrentSourcePlugin(*t, false, *source_peers));
}

shared_ptr<torrent_plugin> create_hash_check_torrent_source_plugin(torrent *t, void *args) {
    map<tcp::endpoint, string> *source_peers = (map<tcp::endpoint, string> *)args;
    return shared_ptr<torrent_plugin>(new TorrentSourcePlugin(*t, true, *source_peers));
}

shared_ptr<torrent_plugin> create_piece_hash_plugin(torrent *t, void *args) {
    return shared_ptr<torrent_plugin>(new TorrentPieceHashPlugin(*t));
}

}  // namespace bbts
