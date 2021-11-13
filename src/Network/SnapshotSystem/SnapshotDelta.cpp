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

SnapshotDelta::SnapshotDelta(const std::vector<uint8_t> &serialized,
                             std::initializer_list<std::pair<int, int>> areas) :
        SnapshotDelta::SnapshotDelta(serialized, std::move(std::vector<std::pair<int, int>>(areas))) {}

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

SnapshotDelta::SnapshotDelta(const std::vector<uint8_t> &serialized, std::initializer_list<int> list) :
        SnapshotDelta::SnapshotDelta(serialized, createPairArrayFromInitList(list)) {

}

SnapshotDelta::SnapshotDelta(const Snapshot &snapshot, const Snapshot &snapshot1, int chunk_size) :
        SnapshotDelta::SnapshotDelta(snapshot, snapshot1, createPairArrayFromInt(chunk_size, snapshot.size())) {
}

SnapshotDelta::SnapshotDelta(const std::vector<uint8_t> &serialized, int snapshotSize, int chunkSize) :
        SnapshotDelta::SnapshotDelta(serialized, createPairArrayFromInt(chunkSize, snapshotSize)) {
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

SnapshotDelta::SnapshotDelta(const std::vector<uint8_t> &serialized,
                             std::vector<std::pair<int, int>> areas) : areas(std::move(areas)) {
    unpack(serialized);
}

void SnapshotDelta::unpack(const std::vector<uint8_t> &vector) {
    size_t bitMaskByteSize = (areas.size() + sizeof(uint8_t) * 8 - 1) / 8;
    //unserialize header
    for (size_t i = 0; i < areas.size(); i++) {
        size_t byteIdx = (bitMaskByteSize * 8 - areas.size() + i) / 8;
        size_t bitIdx = (bitMaskByteSize * 8 - areas.size() + i) % 8;
        bitMask.emplace_back(vector[byteIdx] & (1 << (7 - bitIdx)));
    }
    //unserialize body
    for (size_t i = 0; i < vector.size() - bitMaskByteSize; i++) {
        delta.push_back(vector[i + bitMaskByteSize]);
    }
}

bool SnapshotDelta::operator==(const SnapshotDelta &rhs) const {
    return areas == rhs.areas &&
           bitMask == rhs.bitMask &&
           delta == rhs.delta;
}

bool SnapshotDelta::operator!=(const SnapshotDelta &rhs) const {
    return !(rhs == *this);
}



////std::vector<uint8_t> SnapshotDelta::deSerialize() {
////    return {};
//}
