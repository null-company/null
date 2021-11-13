#include <SnapshotSystem/SnapshotDelta.h>
#include <SnapshotSystem/Snapshot.h>
#include "gtest/gtest.h"

TEST(SnapshotDelta, delta_comparing_1) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8});
    Snapshot snapshot2({4, 2, 3, 18, 19, 6, 7, 133});
    SnapshotDelta s1(snapshot2, snapshot1);
    auto serialized = s1.serialize();

    SnapshotDelta s2(serialized, 8);

    ASSERT_EQ(s1, s2);
}

TEST(SnapshotDelta, delta_comparing_2) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8});
    Snapshot snapshot2({4, 2, 3, 4, 19, 6, 7, 133});
    SnapshotDelta s1(snapshot2, snapshot1, {1, 2, 4, 8});
    auto serialized = s1.serialize();

    SnapshotDelta s2(serialized, {1, 2, 4, 8});

    ASSERT_EQ(s1, s2);
}

TEST(SnapshotDelta, delta_comparing_3) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    Snapshot snapshot2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    SnapshotDelta s1(snapshot2, snapshot1, {{0, 3},
                                            {6, 7},
                                            {9, 11}});
    auto serialized = s1.serialize();

    SnapshotDelta s2(serialized, {{0, 3},
                                  {6, 7},
                                  {9, 11}});

    ASSERT_EQ(s1, s2);
}

TEST(SnapshotDelta, delta_comparing_auto_gen_test) {
    int TEST_COUNT = 1000;
    for (int i = 0; i < TEST_COUNT; i++) {
        std::vector<uint8_t> v1(rand() % 1000 + 1, 0);
        for (int j = 0; j < v1.size(); j++) {
            v1[rand() % v1.size()] = rand() % 256;
        }
        std::vector<uint8_t> v1_(v1);
        int DIFF_POSITION_NUM = rand() % v1.size();
        for (int j = 0; j < DIFF_POSITION_NUM; j++) {
            v1_[rand() % v1_.size()] = rand() % 256;
        }
        Snapshot snapshot1(v1);
        Snapshot snapshot2(v1_);

        SnapshotDelta s1(snapshot1, snapshot2);
        auto serialized = s1.serialize();

        SnapshotDelta s2(serialized, v1.size());

        ASSERT_EQ(s1, s2);
    }
}