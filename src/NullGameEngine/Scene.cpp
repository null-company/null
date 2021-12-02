#include <memory>

#include <Scene.hpp>

namespace null {

    // box2dWorld is initiated under assumption that
    // gravity is default
    Scene::Scene() : box2dWorld(b2Vec2(0.0, 9.8f)) { }

    void Scene::addGameObject(std::unique_ptr<GameObject> newGameObject) {
        newGameObject->scene = self;
        gameObjects.push_back(move(newGameObject));
    }

    void Scene::start() {
        camera.start();
        for (auto &obj : gameObjects) {
            obj->start();
        }
    }

    void Scene::update() {
        camera.update();
        constexpr float timeStep = 1.0f / 60.0f;
        constexpr int velocityIterations = 8;
        constexpr int positionIterations = 3;

        box2dWorld.Step(timeStep, velocityIterations, positionIterations);
        
        for (auto &obj : gameObjects) {
            obj->update();
        }
    }

    b2World& Scene::getBox2dWorld() {
        return box2dWorld;
    }

}

