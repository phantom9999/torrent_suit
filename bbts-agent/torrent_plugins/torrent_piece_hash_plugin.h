

#ifndef GKO_SUIT_TORRENT_PIECE_HASH_PLUGIN_H
#define GKO_SUIT_TORRENT_PIECE_HASH_PLUGIN_H

#include <boost/shared_ptr.hpp>
#include <libtorrent/extensions.hpp>



namespace bbts {

class TorrentPieceHashPlugin : public libtorrent::torrent_plugin {
public:
    explicit TorrentPieceHashPlugin(libtorrent::torrent& t) : _torrent(t) {}

    ~TorrentPieceHashPlugin() override = default;

    boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection* pc) override;

private:
    libtorrent::torrent& _torrent;
};



}


#endif //GKO_SUIT_TORRENT_PIECE_HASH_PLUGIN_H
