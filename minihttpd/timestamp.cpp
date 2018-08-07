#include "minihttpd/timestamp.h"
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS
#include <boost/static_assert.hpp>

namespace argus {
namespace common {

Timestamp::Timestamp(int64_t microseconds)
    : microSecondsSinceEpoch_(microseconds) {
}

Timestamp Timestamp::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::invalid() {
    return Timestamp();
}

} // namespace common
} // namespace argus

