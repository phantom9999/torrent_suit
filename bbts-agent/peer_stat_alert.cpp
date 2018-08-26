#include "peer_stat_alert.h"
#include <string>
#include "process_info.h"


namespace bbts {
std::string PeerStatAlert::message() const {
    char ih_hex[41];
    libtorrent::to_hex((const char*)&_infohash[0], 20, ih_hex);
    boost::system::error_code ec;
    std::string address = _cluster_uri.empty() ? _remote.address().to_string(ec) : _cluster_uri;
    char msg[200];
    snprintf(msg, sizeof(msg), "%s,%s,%s,%ld,%ld,%ld,%ld,%d", ih_hex, g_process_info->ip().c_str(),
             address.c_str(), _start_time, _end_time, _uploaded, _downloaded, !_cluster_uri.empty());
    return msg;
}

std::string PeerStatAlert::infohash() const {
    char ih_hex[41];
    libtorrent::to_hex((const char*)&_infohash[0], 20, ih_hex);
    return ih_hex;
}

std::string PeerStatAlert::local_host() const {
    return g_process_info->ip();
}

std::string PeerStatAlert::remote_host() const {
    boost::system::error_code ec;
    return _cluster_uri.empty() ? _remote.address().to_string(ec) : _cluster_uri;
}






}



