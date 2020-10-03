#include "minihttpd/eventloop.h"
#include "minihttpd/timestamp.h"
#include <sys/time.h>
#include <event2/event.h>
#include <cassert>
#include <boost/thread/lock_guard.hpp>
#include <thread>

#include "common/log.h"

namespace argus {
namespace common {

__thread EventLoop *t_loopInThisThread = nullptr;

__thread int32_t run_times = 0;

EventLoop::EventLoop()
    : base_(::event_base_new()),
      looping_(false),
      threadId_(std::this_thread::get_id()),
      wakeupEvent_(nullptr),
      timerNumsCreated_(0) {
    BLOG(info) << "Using Libevent with backend method "
              << ::event_base_get_method(base_);
    if (t_loopInThisThread) {
        BLOG(fatal) << "Another EventLoop "
                   << (unsigned long) (t_loopInThisThread)
                   << " exists in this thread " << threadId_;
    } else {
        t_loopInThisThread = this;
    }

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, wakeupFd_) < 0) {
        BLOG(fatal) << "EventLoop " << (unsigned long) (this)
                   << " socketpair init failed";
    }

    if (!(0 == ::evutil_make_socket_nonblocking(wakeupFd_[0]) &&
        0 == ::evutil_make_socket_nonblocking(wakeupFd_[1]))) {
        BLOG(error) << "EventLoop " << (unsigned long) (this)
                   << " make_socket_nonblocking failed";
    }

    wakeupEvent_ = ::event_new(base_,
                               wakeupFd_[0],
                               EV_READ | EV_PERSIST,
                               wakeupHandler,
                               this);
    assert(wakeupEvent_);

    if (0 != ::event_add(wakeupEvent_, NULL)) {
        BLOG(fatal) << "EventLoop " << (unsigned long) (this)
                   << " wakeupEvent_ add failed";
    }
}

EventLoop::~EventLoop() {
    freeTimers();
    ::event_del(wakeupEvent_);
    ::event_free(wakeupEvent_);
    ::event_base_free(base_);
    ::close(wakeupFd_[0]);
    ::close(wakeupFd_[1]);
    t_loopInThisThread = NULL;
}

int EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    BLOG(info) << "EventLoop " << (unsigned long) (this) << "start looping";

    uint64_t timerId = newTimerId();
    installTimerEvent(timerId, 1800.0, heartbeat, 0, this);

    // make a heartbeat immediately when eventloop starts
    // timerId = newTimerId();
    // installTimerEvent(timerId, 0.001, heartbeat, 1, this);

    int exitcode = ::event_base_loop(base_, 0);
    looping_ = false;
    return exitcode;
}

uint64_t EventLoop::newTimerId() {
    typedef boost::detail::spinlock SpinLock;
    typedef boost::lock_guard<SpinLock> SpinLockGuard;
    SpinLockGuard lock(lock_);
    ++timerNumsCreated_;
    return timerNumsCreated_;
}

void EventLoop::installTimerEvent(uint64_t timerId,
                                  double interval,
                                  event_callback_fn cb_func,
                                  int type,
                                  void *arg) {
    struct event *timer = NULL;
    // add a persist timer
    if (0 == type) {
        timer = ::event_new(base_, -1, EV_PERSIST, cb_func, arg);
    }
        // add a one-shot timer
    else if (1 == type) {
        timer = ::event_new(base_, -1, 0, cb_func, arg);
    }
    assert(timer);

    struct timeval tv;
    int64_t microSeconds = static_cast<int64_t>(interval * Timestamp::kMicroSecondsPerSecond);
    tv.tv_sec = static_cast<time_t>(interval);
    tv.tv_usec = static_cast<suseconds_t>(microSeconds % Timestamp::kMicroSecondsPerSecond);
    if (0 != ::event_add(timer, &tv)) {
        BLOG(fatal) << "EventLoop::installTimerEvent failed";
    }

    auto timer_it = timer_.find(timerId);
    assert(timer_it == timer_.end());

    timer_[timerId] = timer;

    if (1 == type) {
        Timestamp time(addTime(Timestamp::now(), interval));
        oneShotTimerDeadLine_[timerId] = time.microSecondsSinceEpoch();
    }
}

void EventLoop::cancel(uint64_t timerId) {
    Closure<void>
        *pCb = NewClosure(this, &EventLoop::innerCancel, uint64_t(timerId));
    {
        typedef boost::detail::spinlock SpinLock;
        typedef boost::lock_guard<SpinLock> SpinLockGuard;
        SpinLockGuard lock(lock_);
        pendingFunctors_.push_back(pCb);
    }
    wakeup();
}

void EventLoop::innerCancel(uint64_t timerId) {
    BLOG(info) << "going to cancel timer " << timerId;
    std::map<uint64_t, struct event *>::iterator it;
    it = timer_.find(timerId);
    if (it == timer_.end()) {
        BLOG(error) << "timer " << timerId << "not found";
        return;
    }
    ::event_del(it->second);
    ::event_free(it->second);
    timer_.erase(timerId);
    oneShotTimerDeadLine_.erase(timerId);
}

void EventLoop::freeTimers() {
    std::map<uint64_t, struct event *>::iterator it;
    for (it = timer_.begin(); it != timer_.end();) {
        ::event_del(it->second);
        ::event_free(it->second);
        oneShotTimerDeadLine_.erase(it->first);
        timer_.erase(it++);
    }
}

void EventLoop::doPendingFunctors() {
    std::vector<Closure<void> *> functors;
    {
        typedef boost::detail::spinlock SpinLock;
        typedef boost::lock_guard<SpinLock> SpinLockGuard;
        SpinLockGuard lock(lock_);
        functors.swap(pendingFunctors_);
    }

    for (size_t i = 0; i < functors.size(); ++i) {
        functors[i]->Run();
    }
    if (functors.size()) {
        BLOG(info) << "EventLoop::doPendingFunctors() runned " << functors.size()
                  << " functors";
    }
}

void EventLoop::heartbeat() {
    // doPendingFunctors();

    run_times++;
    BLOG(info) << "EventLoop::heartbeat() start...(" << run_times << "th times)";
    Timestamp current(Timestamp::now());

    std::map<uint64_t, int64_t>::iterator it;
    int64_t delta = 10 * Timestamp::kMicroSecondsPerSecond;

    for (it = oneShotTimerDeadLine_.begin();
         it != oneShotTimerDeadLine_.end();) {
        if (it->second + delta < current.microSecondsSinceEpoch()) {
            auto timer_it = timer_.find(it->first);
            assert(timer_it != timer_.end());
            BLOG(info) << "EventLoop::heartbeat() clean oneshottimer "
                      << it->first;

            ::event_del(timer_it->second);
            ::event_free(timer_it->second);
            timer_.erase(timer_it);
            oneShotTimerDeadLine_.erase(it++);
        } else {
            it++;
        }
    }
    BLOG(info) << "EventLoop::heartbeat() end...(" << run_times << "th times)";
}

void EventLoop::heartbeat(int fd, short event, void *arg) {
    static_cast<EventLoop *>(arg)->heartbeat();
}

int EventLoop::quit() {
    if (!looping_) {
        return 0;
    }
    int flag = ::event_base_loopbreak(base_);
    wakeup();
    return flag;
}

struct event_base *EventLoop::eventBase() {
    return base_;
}

void EventLoop::abortNotInLoopThread() {
    BLOG(fatal) << "EventLoop::abortNotInLoopThread - EventLoop "
               << (unsigned long) (this)
               << " was created in threadId_=" << threadId_
               << " current thread id=" << std::this_thread::get_id();
}

void EventLoop::wakeup() {
    BLOG(info) << "EventLoop::wakeup()  wakeup!!";
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_[1], &one, sizeof one);
    if (n != sizeof one) {
        BLOG(error) << "EventLoop::wakeup() writes " << n << " bytes instead of "
                   << sizeof(one);
    }
}

void EventLoop::wakeupHandler(int fd, short events) {
    BLOG(info) << "EventLoop::wakeupHandler() ok, I heard!!";
    uint64_t one = 1;
    ssize_t n = ::read(fd, &one, sizeof one);
    if (n != sizeof one) {
        BLOG(error) << "EventLoop::wakeupHandler() reads " << n
                   << " bytes instead of " << sizeof(one);
    }

    doPendingFunctors();
}

void EventLoop::wakeupHandler(int fd, short events, void *obj) {
    static_cast<EventLoop *>(obj)->wakeupHandler(fd, events);
}

} // namespace common
} // namespace argus

