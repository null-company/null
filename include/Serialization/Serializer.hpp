#pragma once

#include <string>
#include "Scene.hpp"

namespace null {
    class Serializer {
        static void serializeSceneToFile(Scene* scene, const std::string& filename);
        static Scene getSceneFromFile(const std::string& filename);
    };

}

