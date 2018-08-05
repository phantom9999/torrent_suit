#ifndef ARGUS_COMMON_SPINLOCK_H_
#define ARGUS_COMMON_SPINLOCK_H_

#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/thread/thread_guard.hpp>

namespace argus {
namespace common {


typedef boost::detail::spinlock SpinLock;
typedef boost::lock_guard<SpinLock> SpinLockGuard;

} // namespace common
} // namespace argus


#endif  // ARGUS_COMMON_SPINLOCK_H_
