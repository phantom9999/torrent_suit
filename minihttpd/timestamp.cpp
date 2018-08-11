#include <sys/time.h>
#include "minihttpd/timestamp.h"
namespace argus {
namespace common {

Timestamp::Timestamp(int64_t microseconds)
    : microSecondsSinceEpoch_(microseconds) {
}

Timestamp Timestamp::now() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::invalid() {
    return Timestamp();
}

} // namespace common
} // namespace argus

