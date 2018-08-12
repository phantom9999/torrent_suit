#include "bt_info.hpp"


namespace bbts {

std::vector<FileSlice> BTInfo::map_block(int piece, int64_t offset, int size) const {
    std::vector<libtorrent::file_slice>
        vfs = _info.map_block(piece, offset, size);
    std::vector<FileSlice> nvfs;
    for (auto &vf : vfs) {
        libtorrent::file_entry file_entry = _info.file_at(vf.file_index);
        FileSlice fs;
        fs.path = file_entry.path;
        fs.file_size = file_entry.size;
        fs.pad_file = file_entry.pad_file;
        fs.mode = file_entry.mode;
        fs.file_index = vf.file_index;
        fs.offset = vf.offset;
        fs.size = vf.size;
        nvfs.push_back(fs);
    }
    return nvfs;
}

}





