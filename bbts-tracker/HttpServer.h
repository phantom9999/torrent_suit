#ifndef  BBTS_HTTP_SERVER_H_
#define  BBTS_HTTP_SERVER_H_

#include <string>

#include <boost/thread/thread.hpp>

#include "bbts-tracker/LazySingleton.hpp"

namespace argus {
namespace common {
class MiniHttpd;
class EventLoop;
}
}

namespace bbts {

// wrap mini-httpd and leverage module_status_manager
class HttpServer  {
 public:
  typedef std::string (*callback_fn)(const std::string&);

  // default make object unuseful in constructor
  HttpServer() : httpd_(NULL), loop_(NULL) {}

  bool start(uint16_t port);

  bool SetCallback(const std::string &path, callback_fn cb);

 private:
  // init httpd-server, module_status_manager
  void Run(uint16_t port);

  static std::string QueryMonitorStatusCallBack(const std::string& query);

  // server has been initialized already?
  argus::common::MiniHttpd  *httpd_;
  argus::common::EventLoop  *loop_;
  boost::thread thread_;
};

}  // namespace bbts

#endif  // BBTS_HTTP_SERVER_H_
