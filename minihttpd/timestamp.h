#ifndef ARGUS_COMMON_TIMESTAMP_H_
#define ARGUS_COMMON_TIMESTAMP_H_


#include <cinttypes>
#include <map>


namespace argus {
namespace common {

/// this class is taken from muduo (http://code.google.com/p/muduo/)
/// Time stamp in UTC, in microseconds resolution.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class Timestamp {
public:
    /// Constucts an invalid Timestamp.
    Timestamp() : microSecondsSinceEpoch_(0) {
    }

    ///
    /// Constucts a Timestamp at specific time
    ///
    /// @param microSecondsSinceEpoch
    explicit Timestamp(int64_t microSecondsSinceEpoch);

    void swap(Timestamp &that) {
        ::std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    // for internal usage.
    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

    /// Get time of now.
    static Timestamp now();
    static Timestamp invalid();

    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator>(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() > rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

///
/// Add @c seconds to given timestamp.
///
/// @return timestamp+seconds as Timestamp
///
inline Timestamp addTime(Timestamp timestamp, double seconds) {
    int64_t delta =
        static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

} // namespace common
} // namespace argus

#endif  //  ARGUS_COMMON_TIMESTAMP_H_

