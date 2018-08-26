

#ifndef GKO_SUIT_PEER_SOURCE_REQUEST_FAILED_ALERT_H
#define GKO_SUIT_PEER_SOURCE_REQUEST_FAILED_ALERT_H


#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>


namespace bbts {

class PeerSourceRequestFailedAlert : public libtorrent::peer_alert {
public:
    PeerSourceRequestFailedAlert(const libtorrent::torrent_handle &h,
                                 const libtorrent::tcp::endpoint &ep,
                                 const libtorrent::peer_id &peer_id,
                                 const std::string &error) :
        peer_alert(h, ep, peer_id),
        _error(error) {}

    ~PeerSourceRequestFailedAlert() override = default;

    std::string message() const override {
        return peer_alert::message() + " peer error: " + _error.c_str();
    }

    int type() const override {
        return ALERT_TYPE;
    }

    std::auto_ptr<alert> clone() const override {
        return std::auto_ptr<alert>(new PeerSourceRequestFailedAlert(*this));
    }

    int category() const override {
        return static_category;
    }

    char const* what() const override {
        return "peer_source_request_alert";
    }

public:
    const static int static_category = libtorrent::alert::peer_notification;
    const static int ALERT_TYPE = libtorrent::user_alert_id + 2;

private:
    std::string _error;
};


}

#endif //GKO_SUIT_PEER_SOURCE_REQUEST_FAILED_ALERT_H
