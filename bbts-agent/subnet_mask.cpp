/*#include "bbts-agent/subnet_mask.h"

#include <cstdio>

#include <boost/asio/ip/address.hpp>
#include <boost/system/error_code.hpp>

#include "common/log.h"

using std::string;

namespace bbts {

bool parse_subnet_mask(const string &ip, const string &mask, string *range) {
    using boost::asio::ip::address;
    using boost::system::error_code;
    error_code ec;
    address ip_address = address::from_string(ip, ec);
    if (ec) {
        WARNING_LOG("ip address error: %s\n", ip.c_str());
        return false;
    }
    address mask_address = address::from_string(mask, ec);
    if (ec) {
        WARNING_LOG("mask address error: %s\n", mask.c_str());
        return false;
    }

    unsigned mask_value = static_cast<unsigned>(mask_address.to_v4().to_ulong());
    int i = 31;
    for (; i >= 0; --i) {
        if (((mask_value >> i) & 0x01) == 0) {
            break;
        }
    }

    ++i;
    unsigned long ip_value = ip_address.to_v4().to_ulong();
    unsigned start = static_cast<unsigned>((ip_value >> i) << i);
    unsigned end = static_cast<unsigned>(ip_value | ((1UL << i) - 1));
    unsigned char *p = (unsigned char *)(&start);
    unsigned char *q = (unsigned char *)(&end);
    char buf[32];
    snprintf(buf, sizeof(buf), "%u.%u.%u.%u-%u.%u.%u.%u",
            p[3], p[2], p[1], p[0], q[3], q[2], q[1], q[0]);
    *range = buf;
    return true;
}

}  // namespace bbts
*/