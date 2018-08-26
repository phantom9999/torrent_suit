

#ifndef GKO_SUIT_PEER_STAT_PLUGIN_H
#define GKO_SUIT_PEER_STAT_PLUGIN_H


#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>


namespace bbts {

class PeerStatPlugin : public libtorrent::peer_plugin {
public:
    PeerStatPlugin(libtorrent::peer_connection& pc, libtorrent::torrent& t) :
        _start_time(time(NULL)),
        _peer_connection(pc),
        // _torrent(t),
        _infohash(t.info_hash()),
        _session(t.session()) {}

    ~PeerStatPlugin() override;

    void on_piece_failed(int index) override;

private:
    time_t _start_time;
    libtorrent::peer_connection& _peer_connection;
    // torrent& _torrent;
    // with torrent not a smart ptr, maybe core by destruct
    // so we must get session and infohash first
    libtorrent::sha1_hash _infohash;
    libtorrent::aux::session_impl &_session;
};



}


#endif //GKO_SUIT_PEER_STAT_PLUGIN_H
