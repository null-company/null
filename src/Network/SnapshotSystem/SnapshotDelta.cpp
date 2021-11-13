#include "SnapshotSystem/SnapshotDelta.h"

#include <utility>
#include <numeric>
#include <algorithm>

SnapshotDelta::SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1,
                             std::vector<std::pair<int, int>> areas) : areas(std::move(areas)) {
    fillDelta(snapshot, snapshot1);
}

SnapshotDelta::SnapshotDelta(const Snapshot &snapshot,
                             const Snapshot &snapshot1,
                             std::initializer_list<std::pair<int, int>> areas) :
        SnapshotDelta::SnapshotDelta(snapshot, snapshot1, std::move(std::vector<std::pair<int, int>>(areas))) {}

static std::vector<std::pair<int, int>> createPairArrayFromInitList(std::initializer_list<int> list) {
    std::vector<std::pair<int, int>> result;
    for (auto i = list.begin(); i != list.end(); i++) {
        int l = i == list.begin() ? 0 : *(i - 1);
        int r = *i;
        result.emplace_back(std::pair<int, int>(l, r));
    }
    return result;
};

SnapshotDelta::SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, std::initializer_list<int> list) :
        SnapshotDelta::SnapshotDelta(snapshot, snapshot1, createPairArrayFromInitList(list)) {}

static std::vector<std::pair<int, int>> createPairArrayFromInt(int elem, size_t size) {
    std::vector<std::pair<int, int>> result;
    result.reserve(size);
    for (int i = 0; i < size; i += elem) {
        result.emplace_back(std::pair(i, std::min(i + elem, (int) size)));
    }
    return result;
};

SnapshotDelta::SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, int chunk_size) :
        SnapshotDelta::SnapshotDelta(snapshot, snapshot1, createPairArrayFromInt(chunk_size, snapshot.size())) {
}


void SnapshotDelta::fillDelta(const Snapshot &snapshot, const Snapshot &snapshot1) {
    {
        for (const auto &[from, to]: areas) {
            bool isSame = true;
            for (int i = from; i < to; i++) {
                if (snapshot[i] != snapshot1[i]) {
                    isSame = false;
                    break;
                }
            }
            bitMask.push_back(!isSame);
            if (!isSame) {
                for (int i = from; i < to; i++) {
                    delta.push_back(snapshot1[i]);
                }
            }
        }
    };
}

std::vector<uint8_t> SnapshotDelta::serialize() {
    size_t bitMaskByteSize = (bitMask.size() + sizeof(uint8_t) * 8 - 1) / 8;
    std::vector<uint8_t> serialized(bitMaskByteSize + delta.size(), 0);
    //Serialize header
    for (size_t i = 0; i < bitMask.size(); i++) {
        size_t byteIdx = (bitMaskByteSize * 8 - bitMask.size() + i) / 8;
        size_t bitIdx = (bitMaskByteSize * 8 - bitMask.size() + i) % 8;
        serialized[byteIdx] |= bitMask[i] << (7 - bitIdx);
    }
    //Serialize body
    for (size_t i = 0; i < delta.size(); i++) {
        serialized[bitMaskByteSize + i] = delta[i];
    }
    return serialized;
}


