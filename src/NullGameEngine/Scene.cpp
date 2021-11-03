#include <Scene.hpp>

namespace null
{

    void Scene::addGameObject(GameObject &gameObject) {
        gameObjects.push_back(gameObject);
    }

    void Scene::start() {
        for (auto &obj : gameObjects) {
            obj.start();
        }
    }

    void Scene::update() {
        for (auto &obj : gameObjects) {
            obj.update();
        }
    }

}
