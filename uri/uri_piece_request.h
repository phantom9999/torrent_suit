#ifndef BBTS_AGENT_URI_PIECE_REQUEST_H
#define BBTS_AGENT_URI_PIECE_REQUEST_H

#include <vector>
#include <string>

namespace bbts {

/**
 *
 */
typedef std::vector<char> Buffer;

struct FilePiece {
    std::string name{};
    uint64_t offset{0};
    int32_t size{0};
};

struct URIPieceRequest {
    uint32_t piece_index{0};
    uint32_t piece_offset{0};
    uint32_t size{0};
    uint32_t piece_length{0};
    std::string infohash{};
    std::string root_path{};
    std::vector<FilePiece> file_pieces{};
};

} // namespace bbts

#endif // BBTS_AGENT_URI_PIECE_REQUEST_H
