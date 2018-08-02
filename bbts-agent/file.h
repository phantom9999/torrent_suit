#ifndef BBTS_AGENT_FILE_H
#define BBTS_AGENT_FILE_H

#include <string>
#include <vector>

#include <boost/system/error_code.hpp>

namespace bbts {
class File {
public:
    static bool lock(const std::string &filename, boost::system::error_code &ec);

    static int64_t size(const std::string &filename, boost::system::error_code &ec);

    static int read(const std::string &filename,
            std::vector<char> *buffer,
            boost::system::error_code &ec,
            int64_t limit);

    static int write(const std::string &filename,
            const std::vector<char> &buffer,
            boost::system::error_code &ec);

private:
    File();
    ~File();
};

} // namespace bbts

#endif // BBTS_AGENT_FILE_H
