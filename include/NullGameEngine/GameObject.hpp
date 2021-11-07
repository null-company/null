#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <set>

#include <NullGameEngine.hpp>
#include <Script.hpp>

namespace null {

    class GameObject {
    protected:
        bool isVisible;
        std::shared_ptr<sf::Sprite> sprite;
        std::weak_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;
        std::set<std::string> tags;
        std::vector<Script> scripts;

    public:

        GameObject(std::shared_ptr<sf::Sprite>);

        std::weak_ptr<sf::Sprite> getSprite();

        bool getIsVisible();

        // Returns the children of this GameObject
        // Potentially is VERY expensive!
        std::vector<std::weak_ptr<GameObject>> getChildren();

        // Returns the child of this GameObject by its index
        std::weak_ptr<GameObject> getChild(int index);

        void addChild(const std::shared_ptr<GameObject> &child);

        std::vector<Script> &getScripts();

        void addTag(const std::string &str);

        bool removeTag(const std::string& str);

        sf::Transform getTransform();

        sf::Vector2f getPosition();

        void setPosition(float x, float y);

        void setPosition(sf::Vector2f &pos);

        void start();

        void update();

        void setIsVisible(bool isVisible);

        void addScript(Script &script);
    };

}
