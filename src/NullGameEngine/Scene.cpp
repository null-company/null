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
            for (const auto& child: go.children) {
                walk(*child);
            }
        };

        walk(gameObject);
    }

    void Scene::sceneTreeForEachDo(std::function<void(GameObject&)> function) const {
        for (const auto& obj : rootGameObjects) {
            objectTreeForEachDo(*obj, function);
        }
    }

    void Scene::addRootGameObject(std::shared_ptr<GameObject>&& newGameObject) {
        newGameObject->scene = shared_from_this();
        rootGameObjects.push_back(newGameObject);
    }

    void Scene::start() {
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

}

