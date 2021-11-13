#ifndef NULL_GAME_SNAPSHOT_H
#define NULL_GAME_SNAPSHOT_H

#include <vector>
#include <memory>

class Snapshot {
    using const_iter = std::vector<uint8_t>::const_iterator;
    std::vector<uint8_t> storage;

public:
    explicit Snapshot(std::vector<uint8_t> storage);

    explicit Snapshot(int size);

    uint8_t &operator[](int idx);

    const uint8_t &operator[](int idx) const;

    const_iter begin() const;

    const_iter end() const;

    size_t size() const;
};

#endif
