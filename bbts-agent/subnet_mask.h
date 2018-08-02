#ifndef BBTS_AGENT_SUBNET_MASK_H
#define BBTS_AGENT_SUBNET_MASK_H

#include <string>

namespace bbts {

bool parse_subnet_mask(const std::string &ip, const std::string &mask, std::string *range);

}  // namespace bbts

#endif // BBTS_AGENT_SUBNET_MASK_H
