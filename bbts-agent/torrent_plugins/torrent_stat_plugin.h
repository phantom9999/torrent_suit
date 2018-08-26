

#ifndef GKO_SUIT_TORRENT_STAT_PLUGIN_H
#define GKO_SUIT_TORRENT_STAT_PLUGIN_H

#include <boost/shared_ptr.hpp>
#include <libtorrent/extensions.hpp>

namespace bbts {

class TorrentStatPlugin : public libtorrent::torrent_plugin {
public:
    explicit TorrentStatPlugin(libtorrent::torrent& t) : _torrent(t) {}
    ~TorrentStatPlugin() override = default;
    boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection* pc) override;

private:
    libtorrent::torrent& _torrent;
};


}


#endif //GKO_SUIT_TORRENT_STAT_PLUGIN_H
