#pragma once

#include <memory>
#include <set>

#include <box2d/box2d.h>

#include <SFML/Graphics.hpp>

#include <NullGameEngine.hpp>
#include <Script.hpp>
#include <RenderLayer.hpp>

namespace null {

    class GameObject {
    private:
        void assertSpriteHasSize();
        void setRigidBodyDefPositionBySprite(b2BodyDef&);
        void setShapeAsBoxBySprite(b2PolygonShape&);
    protected:
        sf::Sprite sprite;
        b2Body* rigidBody = nullptr;
        std::weak_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;
        std::set<std::string> tags;
        std::vector<std::unique_ptr<Script>> scripts;

        void start();

        void update();

    public:

        RenderLayer renderLayer;

        GameObject();

        ~GameObject();

        bool visible;

        sf::Sprite& getSprite();

        b2Body* getRigidBody();

        void makeStatic(b2World& box2dWorld);

        void makeDynamic(b2World& box2dWorld);

        void detachFromPhysicsWorld();

        // Returns the children of this GameObject
        // Potentially is VERY expensive!
        std::vector<std::weak_ptr<GameObject>> getChildren();

        // Returns the child of this GameObject by its index
        std::weak_ptr<GameObject> getChild(int index);

        void addChild(const std::shared_ptr<GameObject> &child);

        std::vector<std::unique_ptr<Script>>& getScripts();

        void addTag(const std::string &str);

        bool removeTag(const std::string& str);

        const sf::Transform& getTransform();

        const sf::Vector2f& getPosition();

        void setPosition(float x, float y);

        void setPosition(sf::Vector2f &pos);

        void addScript(std::unique_ptr<Script> script);

        template<class T, typename... Args>
        void addScript(Args&&... args) {
            std::unique_ptr<Script> script =
                std::make_unique<T>(std::forward<Args>(args)...);
            scripts.push_back(std::move(script));
        }

        friend Scene;
    };

}

