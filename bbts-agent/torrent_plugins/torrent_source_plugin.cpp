#include "torrent_source_plugin.h"
#include <libtorrent/peer_connection.hpp>
#include "source_peer_plugin.h"

namespace bbts {

boost::shared_ptr<libtorrent::peer_plugin> TorrentSourcePlugin::new_connection(libtorrent::peer_connection *pc) {
    boost::shared_ptr<libtorrent::peer_plugin> empty_peer_plugin;
    if (pc->type() != libtorrent::peer_connection::bittorrent_connection) {
        return empty_peer_plugin;
    }
    std::map<libtorrent::tcp::endpoint, std::string>::const_iterator
        i = _source_peers.find(pc->remote());
    if (i == _source_peers.end()) {
        return empty_peer_plugin;
    }
    return boost::shared_ptr<libtorrent::peer_plugin>(new SourcePeerPlugin(*pc,
                                                                           _torrent,
                                                                           i->second,
                                                                           _dynamic_hash_check));
}

}


