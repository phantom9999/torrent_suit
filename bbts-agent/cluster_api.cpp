#include "bbts-agent/cluster_api.h"

#include <dlfcn.h>
#include <ftw.h>

#include <vector>

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "common/LazySingleton.hpp"
#include "common/log.h"
#include "bbts-agent/path.h"
#include "bbts-agent/process_info.h"
#include "bbts-agent/string_util.h"

using std::string;
using std::vector;
using boost::shared_ptr;
using bbts::message::DownloadConfigure;

namespace bbts {

namespace detail {
static int append_classpath(
        const char* path,
        const struct stat* statbuf,
        int type,
        struct FTW* ftw_info,
        string *classpath) {
    switch (type) {
    case FTW_F:
    case FTW_SL:
        if (boost::ends_with(path, ".jar")) {
            classpath->append(":");
            classpath->append(path);
        }
        break;

    case FTW_D: {
        classpath->append(":");
        classpath->append(path);
        break;
    }

    default:
        return FTW_STOP;
    }

    return FTW_CONTINUE;
}
} // namespace detail

ClusterAPI::ClusterAPIMap ClusterAPI::_s_api_map;

ClusterAPI* ClusterAPI::get_cluster_api(const std::string &library) {
    shared_ptr<ClusterAPI> api;
    ClusterAPIMap::iterator it = _s_api_map.find(library);
    if (it == _s_api_map.end()) {
        api.reset(new ClusterAPI());
        if (!api->parse_library(library)) {
            return NULL;
        }
        _s_api_map[library] = api;
    } else {
        api = it->second;
    }
    return _s_api_map[library].get();
}

bool ClusterAPI::set_classpath_env() {
    DownloadConfigure *configure = LazySingleton<DownloadConfigure>::instance();
    string classpath(".");
    vector<string> dirs;
    boost::split(dirs, configure->class_path(), boost::is_any_of(":"));
    for (vector<string>::iterator dir = dirs.begin(); dir != dirs.end(); ++dir) {
        Path::nftw(*dir, boost::bind(&detail::append_classpath, _1, _2, _3, _4, &classpath),
                10, FTW_ACTIONRETVAL | FTW_PHYS);
    }
    g_process_info->set_evn("LD_LIBRARY_PATH", configure->ld_library_path());
    g_process_info->set_evn("LIBHDFS_OPTS", configure->libhdfs_opts());
    return g_process_info->set_evn("CLASSPATH", classpath);
}

ClusterAPI::ClusterAPI() :
        read_piece_content(NULL),
        read_file(NULL),
        _cluster_library(NULL),
        _support_cluster(false),
        _connect_cluster(NULL),
        _close_cluster(NULL) {}

ClusterAPI::~ClusterAPI() {
    if (_cluster_library) {
        dlclose(_cluster_library);
        _cluster_library = NULL;
    }
}

bool ClusterAPI::parse_library(const std::string &library) {
    _library_name = library;
    string library_path = g_process_info->root_dir() + "/lib/lib" + _library_name + "tool.so";
    _cluster_library = dlopen(library_path.c_str(), RTLD_LAZY);
    if (!_cluster_library) {
        WARNING_LOG("{}: {}", library_path.c_str(), dlerror());
        return false;
    }

    _connect_cluster = (ConnectClusterFunc)dlsym(_cluster_library, "connect_cluster");
    if (!_connect_cluster) {
        WARNING_LOG("{}", dlerror());
        dlclose(_cluster_library);
        return false;
    }

    _close_cluster = (CloseClusterFunc)dlsym(_cluster_library, "close_cluster");
    if (!_close_cluster) {
        WARNING_LOG("{}", dlerror());
        dlclose(_cluster_library);
        return false;
    }

    read_piece_content = (ReadPieceContentFunc)dlsym(
            _cluster_library, "read_piece_content_from_cluster");
    if (!read_piece_content) {
        WARNING_LOG("{}", dlerror());
        dlclose(_cluster_library);
        return false;
    }

    read_file = (ReadFileFunc)dlsym(_cluster_library, "read_file_from_cluster");
    if (!read_file) {
        WARNING_LOG("{}", dlerror());
        dlclose(_cluster_library);
        return false;
    }

    if (library == "hdfs" && !set_classpath_env()) {
        return false;
    }

    _support_cluster = true;
    return true;
}

void ClusterAPI::connect_thread_func(
        const message::SourceURI &source,
        boost::mutex *mut,
        boost::condition_variable *cond,
        bool *complete,
        void** fs) {
    *fs = _connect_cluster(source.host(), source.port(),
            source.user(), source.passwd(), source.path());
    boost::mutex::scoped_lock lock(*mut);
    *complete = true;
    cond->notify_all();
}

void ClusterAPI::close_cluster(void *fs) {
    if (fs) {
        // exit时libhdfs内部可能已经close了，再次close会出core
        // _close_cluster(fs);
    }
}

void* ClusterAPI::connect_cluster_with_timeout(const message::SourceURI &source, int timeout) {
    std::stringstream strm;
    strm << source.protocol() << "://" << source.user() << ':' << source.passwd() << '@'
            << source.host() << ':' << source.port();
    ConnectionMap::iterator connection_it = _connection_map.find(strm.str());
    if (connection_it != _connection_map.end()) {
        return connection_it->second.get();
    }

    TRACE_LOG("begin to connect cluster({})", strm.str().c_str());
    void *fs = NULL;
    bool complete = false;
    boost::mutex mut;
    boost::condition_variable cond;
    boost::mutex::scoped_lock lock(mut);
    boost::thread connect_thread(boost::bind(&ClusterAPI::connect_thread_func,
            this, source, &mut, &cond, &complete, &fs));
    connect_thread.detach();
    cond.timed_wait(lock, boost::posix_time::seconds(timeout));
    if (fs) {
        TRACE_LOG("connect cluster({}) success!", strm.str().c_str());
        _connection_map[strm.str()] = shared_ptr<void>(fs, boost::bind(&ClusterAPI::close_cluster, this, _1));
    } else if (!complete) {
        WARNING_LOG("conncet cluster({}) timeout: {}s", strm.str().c_str(), timeout);
    } else {
        WARNING_LOG("conncet cluster({}) failed!", strm.str().c_str());
    }
    return fs;
}

}  // namespace bbts
