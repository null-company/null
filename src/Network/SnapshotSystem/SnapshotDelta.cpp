//
// Created by artemonchelus on 07.11.2021.
//

#include "SnapshotSystem/SnapshotDelta.h"

void SnapshotDelta::appendChunk(int pos, std::weak_ptr<uint8_t> *buff) {
    header.busy(pos);
    body.append(buff);
}
