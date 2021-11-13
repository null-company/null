#include <SnapshotSystem/SnapshotDelta.h>
#include <SnapshotSystem/Snapshot.h>
#include "gtest/gtest.h"

TEST(SnapshotDelta, initialization) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8});
    Snapshot snapshot2({4, 2, 3, 18, 19, 6, 7, 133});
    SnapshotDelta s1(snapshot1, snapshot2, {{1, 2},
                                            {2, 3},
                                            {3, 4},
                                            {4, 5}});
    SnapshotDelta s2(snapshot1, snapshot2, {1, 2, 3, 4, 5});
    SnapshotDelta s3(snapshot1, snapshot2, 2);
    SnapshotDelta s4(snapshot1, snapshot2);
}

TEST(SnapshotDelta, default_delta_1) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8});
    Snapshot snapshot2({4, 2, 3, 18, 19, 6, 7, 133});
    SnapshotDelta s1(snapshot1, snapshot2);
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b10011001, 4, 18, 19, 133});


    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, default_delta_2) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8});
    Snapshot snapshot2({4, 2, 3, 18, 19, 6, 7, 133});
    SnapshotDelta s1(snapshot2, snapshot1);
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b10011001, 1, 4, 5, 8});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, default_delta_with_header_size_not_divisible_by_8_1) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6});
    Snapshot snapshot2({4, 2, 3, 18, 19, 6});
    SnapshotDelta s1(snapshot1, snapshot2);
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00100110, 4, 18, 19});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, default_delta_with_no_diff) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6});
    Snapshot snapshot2({1, 2, 3, 4, 5, 6});
    SnapshotDelta s1(snapshot1, snapshot2);
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00000000});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, default_delta_with_header_size_not_divisible_by_8_2) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    Snapshot snapshot2({1, 2, 22, 23, 5, 6, 111, 8, 9, 222});
    SnapshotDelta s1(snapshot1, snapshot2);
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00000000, 0b11001001, 22, 23, 111, 222});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, init_list_delta_1) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    Snapshot snapshot2({1, 2, 100, 4, 5, 6, 7, 80, 9, 10});
    SnapshotDelta s1(snapshot1, snapshot2, {1, 3, 6, 10});
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00000101, 2, 100, 7, 80, 9, 10});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, init_list_delta_2) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 70, 8, 90, 10});
    Snapshot snapshot2({1, 2, 3, 4, 5, 6, 7, 80, 9, 10});
    SnapshotDelta s1(snapshot1, snapshot2, {1, 3, 6, 10});
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00000001, 7, 80, 9, 10});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, init_list_delta_3) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    Snapshot snapshot2({1, 20, 3, 4, 5, 6, 70, 8, 90, 100});
    SnapshotDelta s1(snapshot1, snapshot2, {1, 2, 3, 4, 7});
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00001001, 20, 5, 6, 70});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, init_list_delta_no_diff) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    Snapshot snapshot2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    SnapshotDelta s1(snapshot1, snapshot2, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00000000, 0b00000000});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, fixed_sized_chunk) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    Snapshot snapshot2({1, 2, 100, 4, 5, 6, 7, 8, 9, 111, 11});
    SnapshotDelta s1(snapshot1, snapshot2, 3);
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00001001, 1, 2, 100, 111, 11});
    ASSERT_EQ(serialized, answer);
}

TEST(SnapshotDelta, random_access_areas) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    Snapshot snapshot2({101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112});
    SnapshotDelta s1(snapshot1, snapshot2, {{0,  3},
                                            {6,  10},
                                            {11, 12}});
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00000111, 101, 102, 103, 107, 108, 109, 110, 112});
}


TEST(SnapshotDelta, random_access_areas_2) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    Snapshot snapshot2({101, 102, 103, 104, 105, 106, 7, 8, 109, 110, 111, 112});
    SnapshotDelta s1(snapshot1, snapshot2, {{0,  3},
                                            {6,  8},
                                            {9,  10},
                                            {11, 12}});
    auto serialized = s1.serialize();
    std::vector<uint8_t> answer({0b00001011, 101, 102, 103, 109, 110, 112});
}

