#ifndef NULL_GAME_NULLENGINE_H
#define NULL_GAME_NULLENGINE_H

#include <memory>
#include <set>
#include "SFML/Graphics.hpp"

namespace Null
{
    class GameObject;
    class Script;
    class Component;
    class Scene;
    class SceneLoader;

    class Component {
    private:
        Null::GameObject &gameObject;
    public:
        explicit Component(GameObject &gameObject) : gameObject(gameObject) {}
        virtual void start();
        virtual void update();
    };

    class Script : public Component{
    public:
        virtual void start();
        virtual void update();
    };

    class GameObject{
    private:
        bool isVisible;
        std::shared_ptr<sf::Sprite> sprite;
        std::weak_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;
        std::set<std::string> tags;
        std::vector<Script> scripts;

    public:

        GameObject();

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

        void setIsVisible(bool &isVisible);

        void addScript(Script &script);
    };

    class Scene{
    private:
        GameObject rootGameObject;
        std::vector<GameObject> gameObjects;
    public:
        Scene() {
            gameObjects = std::vector<GameObject>();
            rootGameObject = GameObject();
        }

        void start();

        void update();

        void addGameObject(GameObject &gameObject);
    };

}


#endif //NULL_GAME_NULLENGINE_H
