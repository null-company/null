#ifndef NULL_GAME_SERIALIZABLE_H
#define NULL_GAME_SERIALIZABLE_H

#include <vector>
#include <cctype>

class Serializable {
public:
    virtual std::vector<uint8_t> serialize(std::vector<uint8_t> &in) = 0;
};

#endif