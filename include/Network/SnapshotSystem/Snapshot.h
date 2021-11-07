#ifndef NULL_GAME_SNAPSHOT_H
#define NULL_GAME_SNAPSHOT_H

#include <vector>
#include <memory>
#include "SnapshotDelta.h"

class Snapshot {
    std::vector<uint8_t> storage;

public:
    explicit Snapshot(std::vector<uint8_t> storage);

    explicit Snapshot(int size);
    int size(){
        return storage.size();
    }
};

namespace snapshot {
    SnapshotDelta makeDelta(Snapshot &snapshot1, Snapshot &snapshot2, int chunk_size);

    SnapshotDelta makeDelta(Snapshot &snapshot1);

    Snapshot applyDelta(Snapshot &snapshot, SnapshotDelta &delta);
}
#endif
