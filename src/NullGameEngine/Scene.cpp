#include <memory>

#include <Scene.hpp>

namespace null
{

    void Scene::addGameObject(std::unique_ptr<GameObject> newGameObject) {
        gameObjects.push_back(move(newGameObject));
    }

    void Scene::start() {
        for (auto &obj : gameObjects) {
            obj->start();
        }
    }

    void Scene::update() {
        for (auto &obj : gameObjects) {
            obj->update();
        }
    }

}
