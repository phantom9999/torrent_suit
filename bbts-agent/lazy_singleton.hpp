#ifndef BBTS_AGENT_LAZY_SINGLETON_HPP
#define BBTS_AGENT_LAZY_SINGLETON_HPP

#include <assert.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "bbts/type_lock.hpp"

namespace bbts {

template<typename Type>
class LazySingleton : public boost::noncopyable {
public:
    static Type* instance() {
        if (!_s_instance) {
            TypeLock<LazySingleton<Type> > lock;
            if (!_s_instance) {
                _s_instance.reset(new Type());
            }
        }
        assert(_s_instance);
        return _s_instance.get();
    }

private:
    LazySingleton();
    ~LazySingleton();

    static boost::scoped_ptr<Type> _s_instance;
};

template<typename Type>
boost::scoped_ptr<Type> LazySingleton<Type>::_s_instance;

}  // namespace bbts

#endif // BBTS_AGENT_LAZY_SINGLETON_HPP
