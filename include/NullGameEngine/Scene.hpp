#pragma once

#include <memory>
#include <functional>

#include <box2d/box2d.h>
#include "GameObject.hpp"
#include <Camera.hpp>
#include "WindowMetaInfo.hpp"

namespace null {
    class Scene : public std::enable_shared_from_this<Scene> {
    private:
        mutable std::shared_ptr<GameObject> camera = std::make_shared<GameObject>(std::set<std::string>({"camera"}));
        std::vector<std::shared_ptr<GameObject>> rootGameObjects;
        b2World box2dWorld{b2Vec2(0.0, 9.8f)};
        std::string name = "scene";

        // this function is a crutch;;;;
        // the camera->scene cannot be set to anything other than NULL at init time
        // so you have to call it if you wanna use camera->scene before calling Scene.start()
        // TODO solve this problem
        void setProperCameraScene();
    public:

        mutable WindowMetaInfo windowMetaInfo;

        Scene();

        void objectTreeForEachDo(GameObject&,
                const std::function<void(GameObject&)>&) const;

        void sceneTreeForEachDo(const std::function<void(GameObject&)>&) const;

        std::weak_ptr<GameObject> findFirstByTag(const std::string& tag) const;

        std::vector<std::weak_ptr<GameObject>> findAllByTag(const std::string& tag) const;

        void start();

        void update();

        std::weak_ptr<GameObject> addRootGameObject(std::shared_ptr<GameObject>&&);

        b2World& getBox2dWorld();

        const std::string& getName() const;

        void setName(const std::string&);

        class GameObjectNotFoundException : public std::exception {};

        friend Renderer;

        friend SceneLoader;

        void serialize(google::protobuf::Message &, bool) const;

        static std::shared_ptr<Scene> deserialize(const google::protobuf::Message &);

        WindowMetaInfo& getWindowMetaInfo() const;
    };
}
