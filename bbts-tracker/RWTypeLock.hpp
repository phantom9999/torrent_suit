#ifndef BBTS_RW_TYPE_LOCK_HPP_
#define BBTS_RW_TYPE_LOCK_HPP_

#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace bbts {

template<typename Type>
class RWTypeLock : private boost::noncopyable {
 public:
  RWTypeLock(char lock_type = 'w') : lock_type_(lock_type) {
    if (lock_type_ == 'w') {
      shared_mutex_.lock();
    } else {
      shared_mutex_.lock_shared();
    }
  }
  virtual ~RWTypeLock() {
    if (lock_type_ == 'w') {
      shared_mutex_.unlock();
    } else {
      shared_mutex_.unlock_shared();
    }
  }

 private:
  char lock_type_;

  static boost::shared_mutex shared_mutex_;

  RWTypeLock* operator &();
  RWTypeLock* operator &() const;
};

template<typename Type>
boost::shared_mutex RWTypeLock<Type>::shared_mutex_;

}  // namespace bbts

#endif // BBTS_RW_TYPE_LOCK_HPP_
