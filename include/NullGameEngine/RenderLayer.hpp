#pragma once
//namespace null {
//    // Must be completely migrated to the scene.proto
//    enum RenderLayer {
//        BACKGROUND,
//        BACKGROUND1,
//        BACKGROUND2,
//        BACKGROUND3,
//        FOREGROUND,
//        FOREGROUND1,
//        FOREGROUND2,
//        FOREGROUND3,
//        ONTOP,
//    };
//}
#include "scene.pb.h"

namespace null {
    using RenderLayer = serial::RenderLayer;
}