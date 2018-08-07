#ifndef ARGUS_COMMON_EVENTLOOP_H_
#define ARGUS_COMMON_EVENTLOOP_H_

#include <inttypes.h>
#include "closure.h"
#include "current_thread.h"
#include <map>
#include <queue>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>

struct event_base;
struct event;

namespace argus {
namespace common {

class EventLoop : boost::noncopyable {
public:
    EventLoop();
    ~EventLoop();

    // Loops forever if there are registered events.
    // Must be called in the same thread as creation of the object.
    // return 0 if successful, -1 if an error occurred, or 1 if no events were registered
    int loop();

    // quit the event loop
    // return 0 if successful, or -1 if an error occurred
    int quit();

    // taken from libevent/include/event2/event.h
    // A callback function for an event, It receives three arguments:
    // @param fd An fd or signal
    // @param events One or more EV_* flags
    // @param arg A user-supplied argument.
    // typedef void (*event_callback_fn)(evutil_socket_t, short, void *);
    //
    typedef void (*event_callback_fn)(int, short, void *);

    // Cancels a timer according to timerId
    // Safe to call from other threads.
    void cancel(uint64_t timerId);

    struct event_base *eventBase();

    // internal usage
    void wakeup();

    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const {
        return threadId_ == CurrentThread::tid();
    }

    static EventLoop *getEventLoopOfCurrentThread();

private:
    struct event_base *const base_;
    bool looping_;
    const pid_t threadId_;
    struct event *wakeupEvent_;
    int wakeupFd_[2];

    void installTimerEvent(uint64_t timerId,
                           double interval,
                           event_callback_fn callback,
                           int type,
                           void *arg);
    void innerCancel(uint64_t timerId);

    std::vector<Closure<void> *> pendingFunctors_;
    void doPendingFunctors();

    void heartbeat();
    void freeTimers();
    static void heartbeat(int fd, short event, void *arg);

    std::map<uint64_t, struct event *> timer_;
    std::map<uint64_t, int64_t> oneShotTimerDeadLine_;
    volatile uint64_t timerNumsCreated_;
    uint64_t newTimerId();

    boost::detail::spinlock lock_;

    void abortNotInLoopThread();
    void wakeupHandler(int, short);
    static void wakeupHandler(int, short, void *);
};

} // namespace common
} // namespace argus

#endif  // ARGUS_COMMON_EVENTLOOP_H_

