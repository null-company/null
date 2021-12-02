#pragma once

#include <memory>

#include <box2d/box2d.h>

#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <Camera.hpp>

namespace null {
    class Scene {
    private:
        Camera camera;
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        b2World box2dWorld;
    public:
        Scene();

        std::weak_ptr<Scene> self;

        void start();

        void update();

        void addGameObject(std::unique_ptr<GameObject>);

        b2World& getBox2dWorld();

        friend Renderer;

        friend SceneLoader;
    };
}

