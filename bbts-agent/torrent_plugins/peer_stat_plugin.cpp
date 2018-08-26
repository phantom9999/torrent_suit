#include "peer_stat_plugin.h"
#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_id.hpp>
#include <libtorrent/peer_connection.hpp>

#include "bbts-agent/peer_stat_alert.h"

namespace bbts {


PeerStatPlugin::~PeerStatPlugin() {
    boost::system::error_code ec;
    if (_session.m_alerts.should_post<PeerStatAlert>()) {
        _session.m_alerts.post_alert(
            PeerStatAlert(_infohash,
                          _peer_connection.remote(),
                          _start_time,
                          _peer_connection.statistics().total_payload_upload(),
                          _peer_connection.statistics()
                              .total_payload_download()));
    }
}


void PeerStatPlugin::on_piece_failed(int index) {
    _peer_connection.disconnect(libtorrent::errors::too_many_corrupt_pieces);
}

}

