#ifndef BBTS_AGENT_STATISTICS_H
#define BBTS_AGENT_STATISTICS_H

#include <cstdio>

#include <string>

#include "torrent_plugins/torrent_plugin.h"
#include "peer_stat_file.h"


namespace bbts {

struct TaskStatistics {
    TaskStatistics() = default;

    std::string hostname;
    std::string ip;
    int port{0};
    std::string infohash;
    int64_t total_size{0};
    int64_t payload_downloaded{0};
    int64_t payload_uploaded{0};
    int32_t progress_ppm{0};
    int32_t start_time{0};
    int32_t end_time{0};
    int32_t time_for_download_metadata{0};
    int32_t time_for_check_files{0};
    int32_t time_for_download{0};
    int32_t time_for_seeding{0};
    int32_t upload_limit{0};
    int32_t download_limit{0};
    int32_t retval{0};
    int32_t num_files{0};
    int32_t num_paths{0};
    int32_t num_symlinks{0};
    int32_t piece_length{0};
    int32_t num_pieces{0};
    bool is_hdfs_download{false};
    std::string hdfs_address;
    int64_t downloaded_from_hdfs{0};
    std::string product;
};

inline bool print_task_statistics(const TaskStatistics &stat, const std::string &filename) {
    FILE *fp = fopen(filename.c_str(), "a");
    if (!fp) {
        return false;
    }
    fprintf(fp,
            "%s,%s,%d,%s,%ld,%d,%d,%d,%d,%d,%d,%d,%ld,%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%ld,%s,%s\n",
            stat.hostname.c_str(),
            stat.ip.c_str(),
            stat.port,
            stat.infohash.c_str(),
            stat.total_size,
            stat.piece_length,
            stat.num_pieces,
            stat.num_files,
            stat.num_paths,
            stat.num_symlinks,
            stat.download_limit,
            stat.upload_limit,
            stat.payload_downloaded,
            stat.payload_uploaded,
            stat.progress_ppm,
            stat.start_time,
            stat.end_time,
            stat.retval,
            stat.time_for_download_metadata,
            stat.time_for_check_files,
            stat.time_for_download,
            stat.time_for_seeding,
            stat.is_hdfs_download,
            stat.downloaded_from_hdfs,
            stat.hdfs_address.c_str(),
            stat.product.c_str());
    fclose(fp);
    return true;
}

} // namespace bbts

#endif // BBTS_AGENT_STATISTICS_H
