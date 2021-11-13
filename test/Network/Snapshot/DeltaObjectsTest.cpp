#include <SnapshotSystem/SnapshotDelta.h>
#include <SnapshotSystem/Snapshot.h>
#include "gtest/gtest.h"

TEST(SnapshotDelta, init) {
    Snapshot snapshot1({1, 2, 3, 4, 5, 6, 7, 8});
    Snapshot snapshot2({4, 2, 3, 18, 19, 6, 7, 133});
    SnapshotDelta s1(snapshot1, snapshot2, {{1, 2},
                                            {2, 3},
                                            {3, 4},
                                            {4, 5}});
    SnapshotDelta s2(snapshot1, snapshot2, {1, 2, 3, 4, 5});
    SnapshotDelta s3(snapshot1, snapshot2, 1);
    SnapshotDelta s4(snapshot1, snapshot2);
}

