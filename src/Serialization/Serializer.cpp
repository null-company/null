#include <fstream>
#include <filesystem>
#include <Serializer.hpp>
#include "plog/Log.h"
#include <google/protobuf/util/json_util.h>

namespace null {

    std::unordered_set<Entity*> Serializer::deserializedEntitySet = {};
    std::unordered_map<Entity**, uint64_t> Serializer::toBeSetEntityPointerMap = {};
    std::unordered_map<uint64_t, std::shared_ptr<GameObject>> Serializer::guidToGameObjectMap = {};
    std::unordered_map<uint64_t, std::vector<uint64_t>> Serializer::childrenByGuidMap = {};
    GameObject* Serializer::currentDeserializationGameObject = nullptr;
    std::shared_ptr<Scene> Serializer::currentScene = nullptr;

    void Serializer::serializeSceneToFile(null::Scene* scene) {
        auto const& name = scene->getName();

        std::filesystem::create_directory(name);
        std::filesystem::create_directory(name + "/pbuf");
        std::filesystem::create_directory(name + "/json");
        std::filesystem::create_directory(name + "/pbuf/gameObjects");
        std::filesystem::create_directory(name + "/json/gameObjects");

        auto s_scene = serial::Scene();
        scene->serialize(s_scene, false);
        auto ofstream = std::ofstream(name + "/pbuf/scene.pbuf");
        s_scene.SerializeToOstream(&ofstream);
        ofstream.close();

        auto filename2 = name + "/json/scene.json";
        std::string s;
        ofstream = std::ofstream(filename2);
        scene->serialize(s_scene, true);
        google::protobuf::util::MessageToJsonString(s_scene, &s);
        ofstream << s;
        ofstream.close();
    }

    std::shared_ptr<Scene> Serializer::getSceneFromFile(const std::string& name, bool json) {
        auto s_scene = serial::Scene();

        deserializedEntitySet = {};
        toBeSetEntityPointerMap = {};
        currentDeserializationGameObject = nullptr;
        currentScene = std::make_shared<Scene>();

        // Deserialize all gameObjects
        auto dir_iter = std::filesystem::directory_iterator(
                std::filesystem::path(name + "/" + (json ? "json" : "pbuf") + "/gameObjects")
                );
        for (auto const& i : dir_iter) {
            if (i.is_regular_file()) {
                auto ifs = std::ifstream(i.path());
                serial::GameObject s_go;
                if (i.path().string().ends_with("json")) {
                    std::string s;
                    if (ifs) {
                        std::ostringstream ss;
                        ss << ifs.rdbuf();
                        s += ss.str();
                    }
                    google::protobuf::util::JsonStringToMessage(s, &s_go);
                } else {
                    s_go.ParseFromIstream(&ifs);
                }
                ifs.close();
                auto p_go = GameObject::deserialize(s_go);
                guidToGameObjectMap.try_emplace(p_go->guid, p_go);
            }
        }

        // Get all children
        for (auto const& [guid, child_guid_vec] : childrenByGuidMap) {
            for (auto i : child_guid_vec) {
                guidToGameObjectMap[guid]->addChild(guidToGameObjectMap[i]);
            }
        }

        std::ifstream ifstream = json ?
             std::ifstream(name + "/json/scene.json") :
             std::ifstream(name + "/pbuf/scene.pbuf");
        if (json){
            std::string s;
            if (ifstream) {
                std::ostringstream ss;
                ss << ifstream.rdbuf();
                s += ss.str();
            }
            google::protobuf::util::JsonStringToMessage(s, &s_scene);
        } else {
            s_scene.ParseFromIstream(&ifstream);
        }
        ifstream.close();
        LOGD << "Deserializing scene (" << name << ")";
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
        guidToGameObjectMap = {};
        childrenByGuidMap = {};
        currentDeserializationGameObject = nullptr;
        currentScene = nullptr;
        LOGD << "Deserialization complete!";
        return scene;
    }

    void Serializer::serializeGameObjectToFile(const GameObject* gameObject, const std::string& name) {
        auto s_go = serial::GameObject();
        auto json = name.ends_with("json");
        gameObject->serialize(s_go, json);
        auto ofstream = std::ofstream(name);

        if (json) {
            std::string s;
            google::protobuf::util::MessageToJsonString(s_go, &s);
            ofstream << s;
        } else {
            s_go.SerializeToOstream(&ofstream);
        }
        ofstream.close();
    }

    std::shared_ptr<GameObject> Serializer::getGameObjectFromFile(const std::string& filename) {
        auto ifstream = std::ifstream(filename);
        auto s_go = serial::GameObject();
        if (filename.ends_with("json")) {
            std::string asdf;
            ifstream >> asdf;
            google::protobuf::util::JsonStringToMessage(asdf, &s_go);
        } else {
            s_go.ParseFromIstream(&ifstream);
        }
        ifstream.close();
        return GameObject::deserialize(s_go);
    }

    void Serializer::addToDeserialized(Entity* pEntity) {
        deserializedEntitySet.insert(pEntity);
    }

    void Serializer::addToBeSetPointer(Entity** ppEntity, uint64_t guid) {
        toBeSetEntityPointerMap.try_emplace(ppEntity, guid);
    }

    const std::unordered_map<
            serial::Script::ScriptInstanceCase,
            std::function<std::unique_ptr<Script>(const google::protobuf::Message&)>
    > Serializer::scriptSerializationMap = {
            {serial::Script::ScriptInstanceCase::kPlayerAnimation, PlayerAnimation::deserialize},
            {serial::Script::ScriptInstanceCase::kExampleCameraScript, ExampleCameraScript::deserialize},
            {serial::Script::ScriptInstanceCase::kExampleClockedScript, ExampleClockedScript::deserialize},
            {serial::Script::ScriptInstanceCase::kReloadSceneScript, ReloadSceneScript::deserialize},
            {serial::Script::ScriptInstanceCase::kCameraScript, CameraScript::deserialize},
            {serial::Script::ScriptInstanceCase::kButtonScript, ButtonScript::deserialize},
            {serial::Script::ScriptInstanceCase::kExampleScript, ExampleScript::deserialize},
            {serial::Script::ScriptInstanceCase::kStraightWeaponScript, StraightWeaponScript::deserialize},
            {serial::Script::ScriptInstanceCase::kBulletScript, BulletScript::deserialize},
            {serial::Script::ScriptInstanceCase::kNetworkPlayerScript, NetworkPlayerScript::deserialize},
            {serial::Script::ScriptInstanceCase::kCursorAnimation, CursorAnimation::deserialize},
            {serial::Script::ScriptInstanceCase::kNetworkManagerClientScript, NetworkManagerClientScript::deserialize},
            {serial::Script::ScriptInstanceCase::kNetworkManagerServerScript, NetworkManagerServerScript::deserialize},
            {serial::Script::ScriptInstanceCase::kPlayerControlledBoxClient, PlayerControlledBoxClient::deserialize},
            {serial::Script::ScriptInstanceCase::kPlayerControlledBoxServer, PlayerControlledBoxServer::deserialize},
            {serial::Script::ScriptInstanceCase::kHealthBar, HealthBar::deserialize},
            {serial::Script::ScriptInstanceCase::kHealthBarHolder, HealthBarHolder::deserialize},
            {serial::Script::ScriptInstanceCase::kWeaponGenerator, WeaponGenerator::deserialize},
            {serial::Script::ScriptInstanceCase::kWeaponStorage, WeaponStorage::deserialize},
            {serial::Script::ScriptInstanceCase::kGrenadeBunchScript, GrenadeBunchScript::deserialize},
            {serial::Script::ScriptInstanceCase::kAnimation, Animation::deserialize},
            {serial::Script::ScriptInstanceCase::kRigidBodyAnimation, RigidBodyAnimation::deserialize},
            {serial::Script::ScriptInstanceCase::kMusicManager, MusicManager::deserialize},
            {serial::Script::ScriptInstanceCase::kWall, Wall::deserialize},
    };
}