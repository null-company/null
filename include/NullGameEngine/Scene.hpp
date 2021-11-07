#pragma once

#include <memory>

#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <Camera.hpp>

namespace null {
    class Scene{
    private:
        Camera camera;
        std::vector<std::unique_ptr<GameObject>> gameObjects;
    public:
        Scene() {
            gameObjects = std::vector<std::unique_ptr<GameObject>>();
            camera = Camera();
        }

        void start();

        void update();

        void addGameObject(std::unique_ptr<GameObject>);

        friend Renderer;
    };
}

