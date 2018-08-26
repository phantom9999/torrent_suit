#include "peer_stat_file.h"

namespace bbts {
PeerStatFile::~PeerStatFile() {
    if (_file) {
        fclose(_file);
    }
}

bool PeerStatFile::open(const std::string &filename) {
    _file = fopen(filename.c_str(), "a");
    if (!_file) {
        return false;
    }
    return true;
}

void PeerStatFile::print(const PeerStatAlert &alert) {
    if (_file && (alert.uploaded() != 0 || alert.downloaded() != 0)) {
        fprintf(_file, "%s\n", alert.message().c_str());
        fflush(_file);
    }
}



}


