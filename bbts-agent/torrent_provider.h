#ifndef BBTS_AGENT_TORRENT_PROVIDER_H
#define BBTS_AGENT_TORRENT_PROVIDER_H

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "bbts-agent/routing.h"
#include "TorrentProviderService.h"

namespace bbts {

/**
 * @brief
 */
class TorrentProvider : public Routing {
public:
    TorrentProvider();
    ~TorrentProvider();

    bool get_torrent_file(const std::string &uri, std::vector<char> *buffer);

    bool get_infohash_torrent_file(
            const std::string &infohash,
            std::string *source,
            std::vector<char> *buffer);

    bool upload_torrent_file(
            const std::string &infohash,
            const std::string &source,
            const std::vector<char> &torrent_code);

    bool is_file_no_found() const {
        return _is_file_no_found;
    }
private:
    bool _is_file_no_found;
};

} // namespace bbts
#endif // BBTS_AGENT_TORRENT_PROVIDER_H
