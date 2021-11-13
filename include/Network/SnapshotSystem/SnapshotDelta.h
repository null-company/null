#ifndef NULL_GAME_SNAPSHOTDELTA_H
#define NULL_GAME_SNAPSHOTDELTA_H

#include <memory>
#include <bitset>
#include <vector>
#include "../Serializable.h"
#include "Snapshot.h"

class SnapshotDelta : public Serializable {
private:
    // Areas of the snapshot, that must be watched
    std::vector<std::pair<int, int>> areas;

    /* bitMask of the snapshot areas
     * 1 means that this area was changed
     * 0 means that this area was unchanged
     * */
    std::vector<bool> bitMask;
    std::vector<uint8_t> delta;
    SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, std::vector<std::pair<int, int>> areas);

public:
    std::vector<uint8_t> serialize(std::vector<uint8_t> &in) override;

    SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, std::initializer_list<std::pair<int, int>>);

    SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, std::initializer_list<int>);

    SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, int chunk_size);
private:
    void fillDelta(const Snapshot &snapshot1, const Snapshot &snapshot2);
};


#endif
