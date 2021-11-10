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
        sf::Sprite sprite;
        std::weak_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;
        std::set<std::string> tags;
        std::vector<std::unique_ptr<Script>> scripts;

        void start();

        void update();

    public:

        GameObject();

        sf::Sprite& getSprite();

        bool getIsVisible();

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

        void setIsVisible(bool isVisible);

        void addScript(std::unique_ptr<Script> script);

        template<class T>
        void addScript(GameObject& go) {
            std::unique_ptr<Script> script = std::make_unique<T>(go);
            scripts.push_back(std::move(script));
        }

        friend Scene;
    };

}
