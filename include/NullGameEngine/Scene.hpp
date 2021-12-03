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
        std::vector<std::unique_ptr<GameObject>> rootGameObjects;
        b2World box2dWorld;
        std::vector<const GameObject*> walkObjectTree(const GameObject*) const;
    public:
        Scene();

        std::weak_ptr<Scene> self;

        std::vector<GameObject*> getAllGameObjects() const;

        void start();

        void update();

        void addRootGameObject(std::unique_ptr<GameObject>);

        b2World& getBox2dWorld();

        friend Renderer;
    };
}

