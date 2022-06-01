#include <memory>

#include <Scene.hpp>
#include "plog/Log.h"
#include "Serializer.hpp"
#include "Utility.hpp"

namespace null {

    // box2dWorld is initiated under assumption that
    // gravity is default
    Scene::Scene() {
        camera->scene = weak_from_this();
    }

    void Scene::objectTreeForEachDo(GameObject& gameObject,
                                    const std::function<void(GameObject&)>& function) const {

        std::function<void(GameObject&)> walk;
        walk = [&function, &walk](GameObject& go) -> void {
            function(go);
            for (const auto& childWeakRef: go.getChildren()) {
                auto childP = childWeakRef.lock();
                if (childP == nullptr) {
                    continue;
                }
                walk(*childP);
            }
        };

        walk(gameObject);
    }

    void Scene::sceneTreeForEachDo(const std::function<void(GameObject&)>& function) const {
        for (const auto& obj: rootGameObjects) {
            objectTreeForEachDo(*obj, function);
        }
    }

    std::weak_ptr<GameObject> Scene::findFirstByTag(const std::string& tag) const {
        std::weak_ptr<GameObject> res;
        bool found = false;
        sceneTreeForEachDo([&tag, &res, &found](GameObject& obj) -> void {
            if (found) {
                return;
            }
            if (obj.hasTag(tag)) {
                res = obj.weak_from_this();
                found = true;
            }
        });
        if (res.lock() == nullptr) {
            throw GameObjectNotFoundException();
        }
        return res;
    }

    std::vector<std::weak_ptr<GameObject>> Scene::findAllByTag(const std::string& tag) const {
        std::vector<std::weak_ptr<GameObject>> res;
        sceneTreeForEachDo([&tag, &res](GameObject& obj) -> void {
            if (obj.hasTag(tag)) {
                res.emplace_back(obj.weak_from_this());
            }
        });
        return res;
    }

    std::weak_ptr<GameObject> Scene::addRootGameObject(std::shared_ptr<GameObject>&& newGameObject) {
        newGameObject->scene = weak_from_this();
        objectTreeForEachDo(*newGameObject, [&newGameObject](GameObject& gameObject) {
            gameObject.scene = newGameObject->scene;
        });
        rootGameObjects.push_back(newGameObject);
        return newGameObject;
    }

    void Scene::start() {
        auto cameraCopy = camera;
        addRootGameObject(std::move(cameraCopy));
        sceneTreeForEachDo([this](GameObject& obj) -> void {
            obj.scene = weak_from_this();
        });
        sceneTreeForEachDo([](GameObject& obj) -> void {
            obj.start();
            PLOGD << obj.guid;
        });
    }

    void Scene::update() {
        constexpr float timeStep = 1.0f / 60.0f;
        constexpr int velocityIterations = 8;
        constexpr int positionIterations = 3;

        box2dWorld.Step(timeStep, velocityIterations, positionIterations);

        sceneTreeForEachDo([](GameObject& obj) -> void {
            obj.update();
        });
    }

    b2World& Scene::getBox2dWorld() {
        return box2dWorld;
    }

    WindowMetaInfo& Scene::getWindowMetaInfo() const {
        return windowMetaInfo;
    }

    void Scene::serialize(google::protobuf::Message& message, bool json) const {
        auto msg = (serial::Scene&) message;
        msg.set_camera_guid(camera->guid);
        Serializer::serializeGameObjectToFile(camera.get(), Utility::gameObjectToFilename(camera.get(), json));
        for (auto const& i: rootGameObjects) {
            msg.add_game_object_guid(i->guid);
            Serializer::serializeGameObjectToFile(i.get(), Utility::gameObjectToFilename(i.get(), json));
        }
        message.CopyFrom(msg);
    }

    std::shared_ptr<Scene> Scene::deserialize(const google::protobuf::Message& message) {
        auto scene = Serializer::currentScene;
        auto msg = (const serial::Scene&) message;
        scene->camera = Serializer::guidToGameObjectMap[msg.camera_guid()];
        for (auto const& i: msg.game_object_guid()) {
            scene->addRootGameObject(std::move(Serializer::guidToGameObjectMap[i]));
        }
        return scene;
    }

    const std::string& Scene::getName() const {
        return name;
    }

    void Scene::setName(const std::string& string) {
        this->name = string;
    }

    void Scene::setProperCameraScene() {
        camera->scene = weak_from_this();
    }
}

