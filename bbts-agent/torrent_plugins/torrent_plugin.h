#ifndef BBTS_AGENT_TORRENT_PLUGIN_H
#define BBTS_AGENT_TORRENT_PLUGIN_H

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <libtorrent/extensions.hpp>

namespace bbts {

class LibtorrentPlugin : public libtorrent::plugin {
public:
    explicit LibtorrentPlugin(int release_file_interval) :
        _release_file_interval(release_file_interval) {}

    ~LibtorrentPlugin() override = default;

    void added(boost::weak_ptr<libtorrent::aux::session_impl> impl) override {
        _impl = impl;
    }

    void on_tick() override;

private:
    boost::weak_ptr<libtorrent::aux::session_impl> _impl;
    uint64_t _tick_count{0};
    int _release_file_interval;
};

boost::shared_ptr<libtorrent::torrent_plugin>
create_torrent_stat_plugin(libtorrent::torrent *torrent, void *args);

boost::shared_ptr<libtorrent::torrent_plugin>
create_no_check_torrent_source_plugin(libtorrent::torrent *torrent, void *args);

boost::shared_ptr<libtorrent::torrent_plugin>
create_hash_check_torrent_source_plugin(libtorrent::torrent *torrent, void *args);

boost::shared_ptr<libtorrent::torrent_plugin>
create_piece_hash_plugin(libtorrent::torrent *torrent, void *args);





}  // namespace bbts

#endif // BBTS_AGENT_TORRENT_PLUGIN_H
