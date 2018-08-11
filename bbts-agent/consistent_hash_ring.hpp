#ifndef BBTS_AGENT_CONSISTENT_HASH_RING_HPP
#define BBTS_AGENT_CONSISTENT_HASH_RING_HPP

#include <cassert>
#include <cstdint>
#include <cstdio>

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <boost/unordered_map.hpp>
#include <boost/crc.hpp>


namespace bbts {

template <typename Node>
std::string to_string(const Node &node) {
    std::stringstream strm;
    strm << node;
    return strm.str();
}

template <typename Node>
class ConsistentHashRing {
public:
    typedef std::map<uint32_t, std::string> NodeMap;
    typedef std::pair<Node, int32_t> NodePair;
    typedef std::map<std::string, NodePair> NodeReplicaMap;

    bool empty() {
        return _ring.empty();
    }

    void add_node(const Node &node, int32_t replicas) {
        assert(replicas > 0);
        std::string node_string = to_string(node);
        _replicas[node_string] = std::make_pair(node, replicas);
        for (int32_t r = 0; r < replicas; ++r) {
            std::string vnode(to_string(r) + node_string + to_string(r));

            boost::crc_32_type crc32Type;
            crc32Type.process_bytes(vnode.c_str(), vnode.size());
            uint32_t hash = crc32Type();
            _ring[hash] = node_string;
        }
    }

    void add_node(const Node &node) {
        add_node(node, 100);
    }

    void remove_node(const Node &node) {
        std::string node_string = to_string(node);
        typename NodeReplicaMap::iterator it = _replicas.find(node_string);
        if (it == _replicas.end()) {
            return;
        }

        int32_t replicas = it->second->second;
        _replicas.erase(it);

        for (int32_t r = 0; r < replicas; ++r) {
            std::string vnode(to_string(r) + node_string + to_string(r));


            boost::crc_32_type crc32Type;
            crc32Type.process_bytes(vnode.c_str(), vnode.size());
            uint32_t hash = crc32Type();
            _ring.erase(hash);
        }
    }

    const Node& get_node(const std::string &data) const {
        if (_ring.empty()) {
            return Node();
        }
        boost::crc_32_type crc32Type;
        crc32Type.process_bytes(data.c_str(), data.size());
        uint32_t hash = crc32Type();
        // Look for the first node >= hash
        NodeMap::const_iterator it = _ring.lower_bound(hash);
        if (it == _ring.end()) {
            // Wrapped around; get the first node
            it = _ring.begin();
        }
        return _replicas[it->second].first;
    }

    void get_two_nodes(const std::string &data, std::vector<Node> *selected) const {
        selected->clear();
        if (_ring.empty()) {
            return;
        }

        boost::crc_32_type crc32Type;
        crc32Type.process_bytes(data.c_str(), data.size());
        uint32_t hash = crc32Type();

        // Look for the first node >= hash
        NodeMap::const_iterator it = _ring.lower_bound(hash);
        if (it == _ring.end()) {
            // Wrapped around; get the first node
            it = _ring.begin();
        }
        selected->push_back(_replicas.find(it->second)->second.first);

        NodeMap::const_iterator it_next = it;
        ++it_next;
        if (it_next == _ring.end()) {
            it_next = _ring.begin();
        }
        while (it_next != it) {
            if (it_next->second != it->second) {
                selected->push_back(_replicas.find(it_next->second)->second.first);
                break;
            }
            ++it_next;
            if (it_next == _ring.end()) {
                it_next = _ring.begin();
            }
        }
    }

    void get_all_nodes(const std::string &data, std::vector<Node> *selected) const {
        get_two_nodes(data, selected);
        for (typename NodeReplicaMap::const_iterator it = _replicas.begin();
                it != _replicas.end(); ++it) {
            selected->push_back(it->second.first);
        }
    }

    const Node get_node(const char *data) const {
        std::string datastr(data);
        return get_node(datastr);
    }

    void get_two_nodes(const char *data, std::vector<Node> *selected) const {
        std::string datastr(data);
        get_two_nodes(datastr, selected);
    }

    void clear() {
        _ring.clear();
        _replicas.clear();
    }

private:
    NodeMap _ring;
    NodeReplicaMap _replicas;
};

}  // namespace bbts

#endif // BBTS_AGENT_CONSISTENT_HASH_RING_HPP
