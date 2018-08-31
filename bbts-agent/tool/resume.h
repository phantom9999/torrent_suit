#ifndef BBTS_AGENT_RESUME_H
#define BBTS_AGENT_RESUME_H

#include <string>
#include <vector>

namespace bbts {
namespace tool {

/**
 * @brief
 */
class Resume {
public:
    Resume() = default;

    void set_filename(const std::string &resume_file);

    void load(std::vector<char> *buffer) const;

    void save(const std::vector<char> &buffer) const;

    void remove() const;

    bool check_file() const;

    ~Resume() = default;

private:
    std::string _filename;
    std::string _tmp_filename;
};

} // namespace tool
} // namespace bbts
#endif // BBTS_AGENT_RESUME_H
