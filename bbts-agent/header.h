

#ifndef GKO_SUIT_HEADER_H
#define GKO_SUIT_HEADER_H

#include <cinttypes>

namespace bbts {

/**
 *
 */
class Header {
public:
    /**
     *
     */
    Header() = default;

    /**
     *
     * @param type
     * @param length
     */
    Header(uint32_t type, uint32_t length) {
        assign(type, length);
    }

    /**
     *
     * @return
     */
    bool is_valid() {
        return (_length ^ _type ^ _magic) == _checksum;
    }

    /**
     *
     * @param type
     * @param length
     */
    void assign(uint32_t type, uint32_t length) {
        _length = length;
        _type = type;
        _magic = 47417;
        _checksum = length ^ type ^ _magic;
    }

    /**
     *
     * @return
     */
    uint32_t type() {
        return _type;
    }

    /**
     *
     * @return
     */
    uint32_t length() {
        return _length;
    }

private:
    uint32_t _length{0};
    uint32_t _type{0};
    uint32_t _magic{0};
    uint32_t _checksum{0};
};

}

#endif //GKO_SUIT_HEADER_H
