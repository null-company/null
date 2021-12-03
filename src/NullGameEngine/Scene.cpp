#include <memory>

#include <Scene.hpp>

namespace null {

    // box2dWorld is initiated under assumption that
    // gravity is default
    Scene::Scene() : box2dWorld(b2Vec2(0.0, 9.8f)) { }

    std::vector<const GameObject*> Scene::walkObjectTree(const GameObject* gameObject) const {

        std::vector<const GameObject*> result;
        std::function<void(const GameObject*)> walk;
        walk = [&result,&walk](const GameObject* go) -> void {
            result.push_back(go);
            for (const auto child: go->children) {
                walk(child.get());
            }
        };

        walk(gameObject);

        return result;
    }

    std::vector<GameObject*> Scene::getAllGameObjects() const {
        std::vector<GameObject*> res;

        for (const auto& obj : rootGameObjects) {
            std::vector<const GameObject*> currConst = walkObjectTree(obj.get());
            auto f = [](const GameObject* go) {
                return const_cast<GameObject*>(go);
            };
            std::transform(
                    currConst.begin(),
                    currConst.end(),
                    std::back_inserter(res),
                    f
                    );
        }

        return res;
    }

    void Scene::addGameObject(std::unique_ptr<GameObject> newGameObject) {
        newGameObject->scene = self;
        rootGameObjects.push_back(move(newGameObject));
    }

    void Scene::start() {
        for (auto &obj : getAllGameObjects()) {
            obj->start();
        }
    }

    void Scene::update() {
        constexpr float timeStep = 1.0f / 60.0f;
        constexpr int velocityIterations = 8;
        constexpr int positionIterations = 3;

        box2dWorld.Step(timeStep, velocityIterations, positionIterations);
        
        for (auto &obj : getAllGameObjects()) {
            obj->update();
        }
    }

    b2World& Scene::getBox2dWorld() {
        return box2dWorld;
    }

}

