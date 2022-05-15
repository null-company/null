#include <fstream>
#include <Serializer.hpp>
#include "plog/Log.h"
#include <google/protobuf/util/json_util.h>

namespace null {

    std::unordered_set<Entity*> Serializer::deserializedEntitySet = {};
    std::unordered_map<Entity**, uint64_t> Serializer::toBeSetEntityPointerMap = {};
    GameObject* Serializer::currentDeserializationGameObject = nullptr;
    Scene* Serializer::currentDeserializationScene = nullptr;

    void Serializer::serializeSceneToFile(const null::Scene* scene, const std::string& filename) {
        auto s_scene = serial::Scene();
        scene->serialize(s_scene);
        auto ofstream = std::ofstream(filename);
        s_scene.SerializeToOstream(&ofstream);
        ofstream.close();

        auto filename2 = filename + ".json";
        std::string s;
        ofstream = std::ofstream(filename2);
        google::protobuf::util::MessageToJsonString(s_scene, &s);
        ofstream << s;
        ofstream.close();
    }

    std::shared_ptr<Scene> Serializer::getSceneFromFile(const std::string& filename) {
        auto ifstream = std::ifstream(filename);
        auto s_scene = serial::Scene();

        deserializedEntitySet = {};
        toBeSetEntityPointerMap = {};
        currentDeserializationGameObject = nullptr;
        currentDeserializationScene = nullptr;

        s_scene.ParseFromIstream(&ifstream);
        ifstream.close();
        LOGD << "Deserializing scene (" << filename << ")";
        auto scene = Scene::deserialize(s_scene);

        // Fill every entity pointer requested
        for (auto const& [ptr, guid] : toBeSetEntityPointerMap) {
            for (const auto& entity : deserializedEntitySet) {
                if (entity->guid == guid) {
                    *ptr = entity;
                    break;
                }
            }
        }

        deserializedEntitySet = {};
        toBeSetEntityPointerMap = {};
        currentDeserializationGameObject = nullptr;
        currentDeserializationScene = nullptr;
        LOGD << "Deserialization complete!";
        return scene;
    }

}