#ifndef BBTS_AGENT_URI_PIECE_REQUEST_H
#define BBTS_AGENT_URI_PIECE_REQUEST_H

#include <vector>

namespace bbts {

typedef std::vector<char> Buffer;

struct FilePiece {
    FilePiece() : offset(0), size(0) {}

    std::string name;
    uint64_t offset;
    int32_t size;
};

struct URIPieceRequest {
    URIPieceRequest() : piece_index(0), piece_offset(0), size(0), piece_length(0) {}

    uint32_t piece_index;
    uint32_t piece_offset;
    uint32_t size;
    uint32_t piece_length;
    std::string infohash;
    std::string root_path;
    std::vector<FilePiece> file_pieces;
};

} // namespace bbts

#endif // BBTS_AGENT_URI_PIECE_REQUEST_H
