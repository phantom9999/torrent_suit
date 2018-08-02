#ifndef BBTS_AGENT_TIMER_UTIL_H
#define BBTS_AGENT_TIMER_UTIL_H

#include <string>

#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/function.hpp>

namespace bbts {

void timer_run_once(const std::string &timer_name,
        boost::asio::deadline_timer &timer,
        const boost::posix_time::time_duration &expiry_time,
        boost::function<void ()> callback);

void timer_run_cycle(const std::string &timer_name,
        boost::asio::deadline_timer &timer,
        const boost::posix_time::time_duration &expiry_time,
        boost::function<void (void)> callback);

} // namespace bbts

#endif // BBTS_AGENT_TIMER_UTIL_H
