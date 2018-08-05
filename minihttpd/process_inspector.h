#ifndef ARGUS_COMMON_PROCESSINSPECTOR_H_
#define ARGUS_COMMON_PROCESSINSPECTOR_H_

#include <string>

namespace argus {
namespace common {

class MiniHttpd;

class ProcessInspector {
public:
  void registerCallBacks(MiniHttpd* httpd);

private:
  static std::string baseInfo(const std::string&);
  static std::string pid(const std::string&);
  static std::string procStatus(const std::string&);
  static std::string openedFiles(const std::string&);
  static std::string threads(const std::string&);
  static std::string startTime(const std::string&);
  static std::string account(const std::string&);
  static std::string binaryPath(const std::string&);
};

} // namespace common
} // namespace argus

#endif  // ARGUS_COMMON_PROCESSINSPECTOR_H_

