#include "SnapshotSystem/Snapshot.h"

Snapshot::Snapshot(std::vector<uint8_t> storage) :
        storage(std::move(storage)) {
}

Snapshot::Snapshot(int size) :
        storage(size, 0) {}

uint8_t &Snapshot::operator[](int idx) {
    return storage[idx];
}

const uint8_t &Snapshot::operator[](int idx) const {
    return storage[idx];
}

Snapshot::const_iter Snapshot::end() const {
    return storage.end();
}

Snapshot::const_iter Snapshot::begin() const {
    return storage.begin();
}

size_t Snapshot::size() const {
    return storage.size();
}
