#include "torrent_piece_hash_plugin.h"

#include <libtorrent/peer_connection.hpp>
#include "piece_hash_peer_plugin.h"

namespace bbts {

boost::shared_ptr<libtorrent::peer_plugin> TorrentPieceHashPlugin::new_connection(libtorrent::peer_connection* pc) {
    if (pc->type() != libtorrent::peer_connection::bittorrent_connection) {
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }
    return boost::shared_ptr<libtorrent::peer_plugin>(new PieceHashPeerPlugin(*pc, _torrent));
}


}
