#pragma once

#include <string>
#include <Scene.hpp>
#include <Scripts.hpp>

namespace null {
    class Serializer {
    public:

        // Contains all entities already deserialized
        // Used to set Entity pointers in toBeSetEntityPointerMap
        // You probably don't have to touch this unless you need to modify GameObject/Scene deserialization code
        static std::unordered_set<Entity*> deserializedEntitySet;

        // Maps Entity pointer pointers to guids
        // Use this to get an Entity pointer set in your structure during deserialization time
        // knowing what guid the Entity has
        static std::unordered_map<Entity**, uint64_t> toBeSetEntityPointerMap;

        // Maps guids to GameObjects currently being deserialized
        static std::unordered_map<uint64_t, std::shared_ptr<GameObject>> guidToGameObjectMap;

        // Maps GameObjects to vectors of guids of their children
        // Used to find and set all the children during deserialization
        static std::unordered_map<uint64_t, std::vector<uint64_t>> childrenByGuidMap;

        // GameObject currently being deserialized
        static GameObject* currentDeserializationGameObject;
        // Scene currently being deserialized
        static std::shared_ptr<Scene> currentScene;

        static void serializeSceneToFile(Scene* scene);

        static void serializeGameObjectToFile(const GameObject* gameObject, const std::string& name);

        static std::shared_ptr<Scene> getSceneFromFile(const std::string& name, bool json);

        static std::shared_ptr<GameObject> getGameObjectFromFile(const std::string& filename);

        static void addToDeserialized(Entity*);
        static void addToBeSetPointer(Entity**, uint64_t);

        static const std::unordered_map<
                serial::Script::ScriptInstanceCase,
                std::function<std::unique_ptr<Script>(const google::protobuf::Message&)>
            > scriptSerializationMap;


    };
}
