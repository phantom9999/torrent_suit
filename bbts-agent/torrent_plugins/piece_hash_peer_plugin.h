#ifndef GKO_SUIT_PIECE_HASH_PEER_PLUGIN_H
#define GKO_SUIT_PIECE_HASH_PEER_PLUGIN_H

#include <vector>
#include <libtorrent/extensions.hpp>

namespace bbts {

class PieceHashPeerPlugin : public libtorrent::peer_plugin {
public:
    enum MessageType {
        MESSAGE_PIECE_HASH_REQUEST = 1,
        MESSAGE_PIECE_HASH_RESPONSE = 2,
    };

    enum MessageIndex {
        MESSAGE_INDEX = 40
    };

    PieceHashPeerPlugin(libtorrent::peer_connection& pc, libtorrent::torrent& t) :
        _peer_connection(pc),
        _torrent(t) {}

    ~PieceHashPeerPlugin() override = default;

    bool write_request(libtorrent::peer_request const& r) override;

    void write_piece_hash(int piece);

    bool on_extended(int length, int msg, libtorrent::buffer::const_interval body) override;

private:
    libtorrent::peer_connection& _peer_connection;
    libtorrent::torrent& _torrent;
};




}


#endif //GKO_SUIT_PIECE_HASH_PEER_PLUGIN_H
