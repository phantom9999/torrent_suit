#ifndef GKO_SUIT_TORRENT_SOURCE_PLUGIN_H
#define GKO_SUIT_TORRENT_SOURCE_PLUGIN_H

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include <libtorrent/extensions.hpp>




namespace bbts {

class TorrentSourcePlugin : public libtorrent::torrent_plugin {
public:
    TorrentSourcePlugin(libtorrent::torrent& t, bool hash_check, const std::map<libtorrent::tcp::endpoint, std::string> &source_peers) :
        _torrent(t),
        _dynamic_hash_check(hash_check),
        _source_peers(source_peers) {}

    ~TorrentSourcePlugin() override = default;

    boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection* pc) override;

private:
    libtorrent::torrent& _torrent;
    bool _dynamic_hash_check;
    const std::map<libtorrent::tcp::endpoint, std::string> &_source_peers;
};



}


#endif //GKO_SUIT_TORRENT_SOURCE_PLUGIN_H
