#ifndef ARGUS_COMMON_MINIHTTPD_H_
#define ARGUS_COMMON_MINIHTTPD_H_


#include <map>
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <inttypes.h>
#include <boost/thread/lock_guard.hpp>

struct evhttp;
struct evhttp_request;
struct evhttp_bound_socket;

namespace argus {
namespace common {

class EventLoop;
class ProcessInspector;

class MiniHttpd: boost::noncopyable {
public:
    // if @port binded failed, program will be aborted. be care!!!
    MiniHttpd(EventLoop *loop, uint16_t port);

    MiniHttpd(EventLoop *loop);

    // return true if @port binded successfully, otherwise return false
    bool bind(uint16_t port);

    // if @port binded faild, program will be aborted. be care!!!
    void bindOrDie(uint16_t port);

    // Set response content of request @path.
    // Safe to call from other threads.
    // return false if failed, and @retmsg contains failed reason
    bool setResponse(const std::string &path,
                     const std::string &response,
                     std::string *retmsg = NULL);

    // Set callBack for request @path
    // Safe to call from other threads.
    // return false if failed, and @retmsg contains failed reason
    typedef std::string (*http_callback_fn)(const std::string &);
    bool
    setCallBack(const std::string &path, http_callback_fn cb, std::string *retmsg = NULL);

    // whether logging http response content
    void enableLogging(bool on = true);

    // enable ip filtering
    void enableIpFilter(bool on = true);

    // set the timeout seconds for an HTTP request
    void setTimeout(int32_t seconds);

    int32_t timeout() const { return reqTimeout_; }

    ~MiniHttpd();
    void stop();
    struct evhttp *evHttp();

    static const std::string baseInfoPath;
    static const int kMaxHeaderLen = 8192;

private:
    void requestCallback(struct evhttp_request *);
    static void requestCallback(struct evhttp_request *, void *);

    void
    parseUri(const char *uri, std::string *path, std::string *query, std::string *fragment);
    bool checkPath(const std::string &path);
    void initPathCharacterMap();

    void getSupportedUri(std::string *response);
    bool getFromLocal(const std::string &path, std::string *response);
    bool
    getFromCallBack(const std::string &path, const std::string &query, std::string *response);

    bool inAllowIps(std::string ipstr);

private:
    bool enableLogging_;
    bool enableIpFilter_;
    std::vector<uint32_t> ipRange_;

    boost::scoped_ptr<ProcessInspector> processInspector_;
    EventLoop *loop_;
    struct evhttp *const evhttp_;
    struct evhttp_bound_socket *boundSocket_;
    int32_t reqTimeout_;
    std::map<char, int8_t> pathCharacterMap_;

    std::map<std::string, http_callback_fn> uriHandlerMap_;
    std::map<std::string, std::string> responseMap_;

    typedef boost::detail::spinlock SpinLock;
    typedef boost::lock_guard<SpinLock> SpinLockGuard;
    boost::detail::spinlock lock_;

};

} // namespace common
} // namespace argus

#endif  // ARGUS_COMMON_MINIHTTPD_H_

