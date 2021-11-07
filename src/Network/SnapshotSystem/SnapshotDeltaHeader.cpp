#include "SnapshotSystem/SnapshotDeltaHeader.h"

SnapshotDeltaHeader::SnapshotDeltaHeader(int snapShotSize, int chunkSize) :
        chunkSize(chunkSize),
        snapshotSize(snapShotSize),
        header(snapShotSize + (chunkSize - 1) / chunkSize, false) {}

void SnapshotDeltaHeader::free(int chunkIdx) {
    header[chunkIdx] = false;
}

void SnapshotDeltaHeader::busy(int chunkIdx) {
    header[chunkIdx] = true;
}

void SnapshotDeltaHeader::serialize(std::weak_ptr<uint8_t> buff) const{

}

int SnapshotDeltaHeader::getChunkSize() const {
    return chunkSize;
}

int SnapshotDeltaHeader::getSnapshotSize() const {
    return snapshotSize;
}
