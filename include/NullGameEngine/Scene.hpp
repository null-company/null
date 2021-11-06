#pragma once

#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <Camera.hpp>

namespace null {
    class Scene{
    private:
        Camera camera;
        std::vector<GameObject> gameObjects;
    public:
        Scene() {
            gameObjects = std::vector<GameObject>();
            camera = Camera();
        }

        void start();

        void update();

        void addGameObject(GameObject &gameObject);

        friend Renderer;
    };
}

