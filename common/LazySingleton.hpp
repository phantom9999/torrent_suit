#ifndef BBTS_LAZY_SINGLETON_HPP_
#define BBTS_LAZY_SINGLETON_HPP_

#include <cassert>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "common/TypeLock.hpp"

namespace bbts {

template<typename Type>
class LazySingleton: private boost::noncopyable {
public:
    typedef TypeLock<LazySingleton<Type> > Lock;

    static Type *instance() {
        if (!instance_) {
            Lock lock;
            if (!instance_) {
                instance_.reset(new Type());
            }
        }
        assert(instance_);
        return instance_.get();
    }

    // construct object with arg1
    template<typename Arg1>
    static Type *instance(const Arg1 &arg1) {
        if (!instance_) {
            Lock lock;
            if (!instance_) {
                instance_.reset(new Type(arg1));
            }
        }
        assert(instance_);
        return instance_.get();
    }

    template<typename Arg1, typename Arg2>
    static Type *instance(const Arg1 &arg1, const Arg2 &arg2) {
        if (!instance_) {
            Lock lock;
            if (!instance_) {
                instance_.reset(new Type(arg1, arg2));
            }
        }
        assert(instance_);
        return instance_.get();
    }

    template<typename Arg1, typename Arg2, typename Arg3>
    static Type *
    instance(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3) {
        if (!instance_) {
            Lock lock;
            if (!instance_) {
                instance_.reset(new Type(arg1, arg2, arg3));
            }
        }
        assert(instance_);
        return instance_.get();
    }

private:
    static boost::scoped_ptr<Type> instance_;

    LazySingleton() = default;
    ~LazySingleton() = default;
};

template<typename Type>
boost::scoped_ptr<Type> LazySingleton<Type>::instance_;

}  // namespace bbts

#endif // BBTS_LAZY_SINGLETON_HPP_
