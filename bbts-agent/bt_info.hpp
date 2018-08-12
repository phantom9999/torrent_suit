#ifndef BBTS_AGENT_BT_INFO_H
#define BBTS_AGENT_BT_INFO_H

#include "bbts-agent/plugin.h"

#include <libtorrent/torrent_info.hpp>

namespace bbts {

class BTInfo : public BTInfoInterface {
public:
    BTInfo(const libtorrent::torrent_info& info, const std::string &path) :
            _info(info),
            _path(path) {}

    ~BTInfo() override = default;

    std::vector<FileSlice> map_block(int piece, int64_t offset, int size) const override;

    int piece_size(unsigned int index) const override {
        return _info.piece_size(index);
    }

    std::string get_prefix_path() const override {
        return _path;
    }

    inline int64_t total_size() const {
        return _info.total_size();
    }

    inline int piece_length() const {
        return _info.piece_length();
    }

    inline int num_pieces() const {
        return _info.num_pieces();
    }

    inline int num_files() const {
        return _info.num_files();
    }

    inline libtorrent::sha1_hash hash_for_piece(int index) const {
        return _info.hash_for_piece(index);
    }

private:
    libtorrent::torrent_info _info;
    std::string _path;
};

}  // namespace bbts

#endif // BBTS_AGENT_BT_INFO_H
