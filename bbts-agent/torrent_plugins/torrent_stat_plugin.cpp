#include "torrent_stat_plugin.h"

#include <boost/shared_ptr.hpp>

#include <libtorrent/peer_connection.hpp>

#include "peer_stat_plugin.h"

namespace bbts {
boost::shared_ptr<libtorrent::peer_plugin> TorrentStatPlugin::new_connection(libtorrent::peer_connection *pc) {
    if (pc->type() != libtorrent::peer_connection::bittorrent_connection) {
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }
    return boost::shared_ptr<libtorrent::peer_plugin>(new PeerStatPlugin(*pc, _torrent));
}


}

