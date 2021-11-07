#ifndef NULL_GAME_SNAPSHOTDELTA_H
#define NULL_GAME_SNAPSHOTDELTA_H
#include "SnapshotDeltaHeader.h"
#include "SnapshotDeltaBody.h"
class SnapshotDelta{
private:
    SnapshotDeltaHeader header;
    SnapshotDeltaBody body;

    void appendChunk(int pos, std::weak_ptr<uint8_t> *buff);

};


#endif
