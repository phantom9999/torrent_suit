

#ifndef GKO_SUIT_SOURCE_PEER_PLUGIN_H
#define GKO_SUIT_SOURCE_PEER_PLUGIN_H


#include <string>

#include <libtorrent/extensions.hpp>



namespace bbts {
class SourcePeerPlugin : public libtorrent::peer_plugin {
public:
    enum MessageType {
        MESSAGE_URI_PIECE_REQUEST = 1,
        MESSAGE_URI_PIECE_FAILED = 2,
        MESSAGE_URI_PIECE_HASH_REQUEST  = 3,
        MESSAGE_URI_PIECE_HASH_RESPONSE  = 4,
    };

    enum MessageIndex {
        MESSAGE_INDEX = 20
    };

    SourcePeerPlugin(libtorrent::peer_connection& pc, libtorrent::torrent& t, std::string source_path, bool dynamic_hash_check) :
        _peer_connection(pc),
        _torrent(t),
        _source_path(source_path),
        _dynamic_hash_check(dynamic_hash_check) {
    }

    ~SourcePeerPlugin() override = default;

    void tick() override;

    void add_handshake(libtorrent::entry &e) override;

    bool write_request(libtorrent::peer_request const& r) override;

    bool on_extended(int length, int msg, libtorrent::buffer::const_interval body) override;

private:
    bool write_request_with_type(int type, libtorrent::peer_request const &r);

private:

    libtorrent::peer_connection& _peer_connection;
    libtorrent::torrent& _torrent;
    std::string _source_path;
    bool _dynamic_hash_check;
    bool _tick_once{false};
};




}


#endif //GKO_SUIT_SOURCE_PEER_PLUGIN_H
