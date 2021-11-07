#include "SnapshotSystem/Snapshot.h"

Snapshot::Snapshot(std::vector<uint8_t> storage) :
        storage(std::move(storage)) {
}

Snapshot::Snapshot(int size) :
        storage(size, 0) {}

SnapshotDelta snapshot::makeDelta(Snapshot &snapshot1, Snapshot &snapshot2, int chunk_size) {
    if(snapshot1.size() != snapshot2.size()){
        throw std::invalid_argument("snapshots have different sizes");
    }
    SnapshotDelta snapshotDelta();
    return;
}
