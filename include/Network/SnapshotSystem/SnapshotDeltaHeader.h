#ifndef NULL_GAME_SNAPSHOTDELTAHEADER_H
#define NULL_GAME_SNAPSHOTDELTAHEADER_H

#include <memory>
#include <bitset>
#include <vector>

class SnapshotDeltaHeader {
private:
    int chunkSize;
    int snapshotSize;
    std::vector<bool> header;
public:
    void free(int chunkIdx);

    void busy(int chunkIdx);

    int getChunkSize() const;

    int getSnapshotSize() const;

    explicit SnapshotDeltaHeader(int snapShotSize, int chunkSize = 1);

    void serialize(std::weak_ptr<uint8_t> buff) const;
};


#endif
