#pragma once

#include <string>
#include "../NullGameEngine/Scene.hpp"

namespace null {
    class Serializer {
    public:
        static std::unordered_set<Entity*> deserializedEntitySet;
        static std::unordered_map<Entity**, uint64_t> toBeSetEntityPointerMap;
        static GameObject* currentDeserializationGameObject;
        static Scene* currentDeserializationScene;

        static void serializeSceneToFile(const Scene* scene, const std::string& filename);
        static std::shared_ptr<Scene> getSceneFromFile(const std::string& filename);


    };
}

