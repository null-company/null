#pragma once

#include <memory>
#include <functional>

#include <box2d/box2d.h>

#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <Camera.hpp>
#include "WindowMetaInfo.hpp"

namespace null {
    class Scene : public std::enable_shared_from_this<Scene> {
    private:
        mutable WindowMetaInfo windowMetaInfo;
        mutable std::shared_ptr<GameObject> camera = std::make_shared<GameObject>(std::set<std::string>({"camera"}));
        std::vector<std::shared_ptr<GameObject>> rootGameObjects;
        b2World box2dWorld;
    public:
        Scene();

        void objectTreeForEachDo(GameObject&,
                                 std::function<void(GameObject&)>) const;

        void sceneTreeForEachDo(std::function<void(GameObject&)>) const;

        std::weak_ptr<GameObject> findFirstByTag(const std::string& tag);

        std::vector<std::weak_ptr<GameObject>> findAllByTag(const std::string& tag);

        void start();

        void update();

        std::weak_ptr<GameObject> addRootGameObject(std::shared_ptr<GameObject>&&);

        b2World& getBox2dWorld();

        class GameObjectNotFoundException : public std::exception {};

        friend Renderer;

        friend SceneLoader;

        WindowMetaInfo& getWindowMetaInfo() const;
    };
}
