#ifndef NULL_GAME_SNAPSHOTMANAGER_H
#define NULL_GAME_SNAPSHOTMANAGER_H

#include <array>
#include <vector>
#include <memory>

#define SNAPSHOT_SIZE 32

class SnapshotManager {
private:
    std::array<std::vector<uint8_t>, SNAPSHOT_SIZE> snapshots;

    std::weak_ptr<uint8_t> data;
    ssize_t size;
    ssize_t lastSnapshotPosition;

};

#endif
