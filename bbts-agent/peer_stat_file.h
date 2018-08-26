

#ifndef GKO_SUIT_PEER_STAT_FILE_H
#define GKO_SUIT_PEER_STAT_FILE_H

#include <string>

#include "peer_stat_alert.h"


namespace bbts {


class PeerStatFile {
public:
    PeerStatFile() = default;

    ~PeerStatFile();

    bool open(const std::string &filename);

    void print(const PeerStatAlert &alert);

private:
    FILE *_file{nullptr};
};

}

#endif //GKO_SUIT_PEER_STAT_FILE_H
