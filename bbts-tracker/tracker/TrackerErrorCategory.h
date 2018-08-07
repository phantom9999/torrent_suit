#ifndef BBTS_TRACKER_ERROR_CATEGORY_H_
#define BBTS_TRACKER_ERROR_CATEGORY_H_

#include <boost/system/error_code.hpp>

namespace bbts {
namespace tracker {

namespace errors {
/* error code */
enum error_code_t {
  NO_ERROR = 0,
  MISSING_ARGS,
  INVALID_ARGS,
  INFOHASH_NOT_FOUND,
  INVALID_OPERAT_TYPE,
  MAX_ERROR_NUM,
};

}

/**
 * @brief
 */
class TrackerErrorCategory : public boost::system::error_category {
 public:
  TrackerErrorCategory() {}
  virtual ~TrackerErrorCategory(){}
  virtual const char* name() const BOOST_SYSTEM_NOEXCEPT{
    return name_;
  }

  virtual std::string message(int ev) const;

 private:
  static const char *name_;
};

const boost::system::error_category& get_error_category();

}  // namespace tracker
}  // namespace bbts
#endif // BBTS_TRACKER_ERROR_CATEGORY_H_
