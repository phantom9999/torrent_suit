#ifndef GKO_SUIT_PEER_STAT_ALERT_H
#define GKO_SUIT_PEER_STAT_ALERT_H


#include <libtorrent/alert.hpp>
#include <libtorrent/peer_id.hpp>
#include <libtorrent/union_endpoint.hpp>
#include <libtorrent/alert_types.hpp>

namespace bbts {

class PeerStatAlert : public libtorrent::alert {
public:
    PeerStatAlert(const libtorrent::sha1_hash &infohash,
                  const libtorrent::tcp::endpoint &ep,
                  time_t start_time,
                  int64_t uploaded,
                  int64_t downloaded) :
        _infohash(infohash),
        _remote(ep),
        _start_time(start_time),
        _end_time(time(NULL)),
        _uploaded(uploaded),
        _downloaded(downloaded) {}

    PeerStatAlert(const libtorrent::sha1_hash &infohash,
                  time_t start_time,
                  int64_t uploaded,
                  int64_t downloaded,
                  std::string cluster_uri) :
        _infohash(infohash),
        _start_time(start_time),
        _end_time(time(NULL)),
        _uploaded(uploaded),
        _downloaded(downloaded),
        _cluster_uri(cluster_uri) {}

    ~PeerStatAlert() override = default;

    int type() const override {
        return ALERT_TYPE;
    }

    std::auto_ptr<alert> clone() const override {
        return std::auto_ptr<alert>(new PeerStatAlert(*this));
    }

    int category() const override {
        return static_category;
    }

    char const* what() const override {
        return "peer_stat_alert";
    }

    std::string message() const override;

    std::string infohash() const;

    std::string remote_host() const;

    std::string local_host() const;

    time_t start_time() const {
        return _start_time;
    }

    time_t end_time() const {
        return _end_time;
    }

    int64_t uploaded() const {
        return _uploaded;
    }

    int64_t downloaded() const {
        return _downloaded;
    }

    bool is_cluster() const {
        return !_cluster_uri.empty();
    }

    const libtorrent::tcp::endpoint& remote() const {
        return _remote;
    }

public:
    const static int static_category = libtorrent::alert::peer_notification;
    const static int ALERT_TYPE = libtorrent::user_alert_id + 1;

private:
    libtorrent::sha1_hash _infohash;
    libtorrent::tcp::endpoint _remote;
    time_t _start_time;
    time_t _end_time;
    int64_t _uploaded;
    int64_t _downloaded;
    std::string _cluster_uri;
};



}


#endif //GKO_SUIT_PEER_STAT_ALERT_H
