#include <memory>

#include <Scene.hpp>

namespace null {

    // box2dWorld is initiated under assumption that
    // gravity is default
    Scene::Scene() : box2dWorld(b2Vec2(0.0, 9.8f)) { }

    void Scene::objectTreeForEachDo(GameObject& gameObject,
                                    std::function<void(GameObject&)> function) const {

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

    void Scene::sceneTreeForEachDo(std::function<void(GameObject&)> function) const {
        for (const auto& obj: rootGameObjects) {
            objectTreeForEachDo(*obj, function);
        }
    }

    std::weak_ptr<GameObject> Scene::findFirstByTag(const std::string& tag) {
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

    std::vector<std::weak_ptr<GameObject>> Scene::findAllByTag(const std::string& tag) {
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
        objectTreeForEachDo(*newGameObject, [&](GameObject& gameObject) {
            gameObject.scene = newGameObject->scene;
        });
        rootGameObjects.push_back(newGameObject);
        return newGameObject;
    }

    void Scene::start() {
        auto cameraCopy = camera;
        addRootGameObject(std::move(cameraCopy));
        sceneTreeForEachDo([](GameObject& obj) -> void {
            obj.start();
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

    void Scene::serialize(google::protobuf::Message* message) {
        auto msg = (serial::Scene*) message;
        for (auto& i: rootGameObjects) {
            auto s_go = msg->add_game_object();
            i->serialize(s_go);
        }
    }

    void Scene::deserialize(google::protobuf::Message* message) {
        auto msg = (serial::Scene*) message;
        for (auto i: msg->game_object()) {
            auto new_go = std::make_shared<GameObject>();
            new_go->deserialize(&i);
        }
    }

}

