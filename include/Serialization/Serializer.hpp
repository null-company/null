#pragma once

#include <string>
#include <Scene.hpp>
#include <Scripts.hpp>

namespace null {
    class Serializer {
    public:

        static std::unordered_set<Entity*> deserializedEntitySet;
        static std::unordered_map<Entity**, uint64_t> toBeSetEntityPointerMap;
        static GameObject* currentDeserializationGameObject;
        static Scene* currentDeserializationScene;

        static void serializeSceneToFile(const Scene* scene, const std::string& filename);
        static std::shared_ptr<Scene> getSceneFromFile(const std::string& filename);
        static void addToDeserialized(Entity*);
        static void addToBeSetPointer(Entity**, uint64_t);

        static const std::unordered_map<
                serial::Script::ScriptInstanceCase,
                std::function<std::unique_ptr<Script>(const google::protobuf::Message&)>
            > scriptSerializationMap;
    };
}
