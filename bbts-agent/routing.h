#ifndef BBTS_AGENT_ROUTING_H
#define BBTS_AGENT_ROUTING_H

#include <string>
#include <sstream>

#include <boost/noncopyable.hpp>

#include "bbts-agent/consistent_hash_ring.hpp"
#include "pb_config/routing.pb.h"

namespace bbts {

namespace message {
class RoutingArea;
}

/**
 * @brief
 */
class Routing : public boost::noncopyable {
public:
    typedef std::pair<std::string, int> Node;
    typedef std::vector<Node> NodeVector;

    Routing() = default;
    virtual ~Routing() = default;

    bool load_conf(const std::string &conf_path, const std::string &machine_room);

    void get_nodes(const std::string &info, NodeVector *nodes) const;

    /**
     * get machine area
     * @return area name of this machine room, e.g. hd, hb
     */
    std::string get_machine_room_area(const std::string &machine_room);

    /**
     * if machine_room in hb, hd or hn, return false
     * else, machine_room may be a domain, return true
     */
    bool check_machine_room_area_default(const std::string &machine_room);

    void set_service_name(const std::string &service_name) {
        if (service_name == "tracker") {
            _service_index = 0;
        } else if (service_name == "provider") {
            _service_index = 1;
        } else if (service_name == "transfer") {
            _service_index = 2;
        } else if (service_name == "bbts_stat") {
            _service_index = 3;
        }
    }

    void set_routing_conf(const std::string &conf_file) {
        _conf_file.assign(conf_file);
    }

    const std::string &routing_conf() const {
        return _conf_file;
    }

private:
    static const char *_s_default_inner_machine_room_area;
    static const char *_s_default_machine_room_area;

    void load_area(const message::RoutingArea& area);

    /**
     * if machine_room is default, return _s_default_inner_machine_room_area
     * instead of _s_default_machine_room_area
     */
    std::string get_machine_room_area_inner(const std::string &machine_room);

    ConsistentHashRing<Node> _hashring;
    message::RoutingConf _routing_conf;
    int _service_index{0};
    std::string _conf_file;
};

} // namespace bbts
#endif // BBTS_AGENT_ROUTING_H
