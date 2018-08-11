#ifndef BBTS_TYPE_LOCK_HPP_
#define BBTS_TYPE_LOCK_HPP_

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

namespace bbts {

template<typename Type>
class TypeLock: private boost::noncopyable {
public:
    // TypeLock lock;
    TypeLock() : lock_(mutex_) {}
    ~TypeLock() = default;

    // TypeLock<T>::lock();
    static void lock() {
        mutex_.lock();
    }

    // TypeLock<T>::unlock();
    static void unlock() {
        mutex_.unlock();
    }

private:
    boost::mutex::scoped_lock lock_;

    static boost::mutex mutex_;

    TypeLock *operator&();
    TypeLock *operator&() const;
};

template<typename Type>
boost::mutex TypeLock<Type>::mutex_;

}  // namespace bbts

#endif // BBTS_TYPE_LOCK_HPP_
