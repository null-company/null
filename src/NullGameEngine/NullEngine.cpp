#include "NullEngine.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

namespace Null
{
    // GameObject definitions start
    // Constructors
    GameObject::GameObject():
    isVisible(false),
    sprite(nullptr) {
        this->children = std::vector<std::shared_ptr<GameObject>>();
        this->tags = std::set<std::string>();
        this->scripts = std::vector<Script>();
    }

    // Getters, Setters and all the like

    std::weak_ptr<sf::Sprite> GameObject::getSprite() {
        return std::weak_ptr<sf::Sprite>(this->sprite);
    }

    bool GameObject::getIsVisible() {
        return this->isVisible;
    }

    void GameObject::setIsVisible(bool &isVisible){
        this->isVisible = isVisible;
    }

    std::vector<std::weak_ptr<GameObject>> GameObject::getChildren() {
        auto result = std::vector<std::weak_ptr<GameObject>>();
        for (const auto& child_ref : this->children) {
            result.push_back(std::weak_ptr<GameObject>(child_ref));
        }
        return result;
    }

    std::weak_ptr<GameObject> GameObject::getChild(int index) {
        return std::weak_ptr(this->children[index]);
    }

    void GameObject::addChild(const std::shared_ptr<GameObject> &child) {
        children.push_back(child);
    }

    std::vector<Script> &GameObject::getScripts() {
        return scripts;
    }

    void GameObject::addScript(Script &script) {
        scripts.push_back(script);
    }

    void GameObject::addTag(const std::string &str) {
        tags.insert(str);
    }

    bool GameObject::removeTag(const std::string &str) {
        auto result = !tags.find(str)->empty();
        tags.erase(str);
        return result;
    }

    sf::Transform GameObject::getTransform() {
        return sprite->getTransform();
    }

    sf::Vector2f GameObject::getPosition() {
        return sprite->getPosition();
    }

    void GameObject::setPosition(float x, float y) {
        sprite->setPosition(x, y);
    }

    void GameObject::setPosition(sf::Vector2f &pos) {
        sprite->setPosition(pos);
    }

    // Non-getter/setter member functions

    void GameObject::start() {
        for (auto &script : scripts) {
            script.start();
        }
    }

    void GameObject::update() {
        for (auto &script : scripts) {
            script.update();
        }
    }

    // GameObject definitions end

    // Component definitions start
    // Component definitions end

    // Script definitions start

    void Script::start(){}
    void Script::update(){}

    // Script definitions end

    // SceneLoader definitions start

    class SceneLoader {

    };

    // SceneLoader definitions end

    // Scene definitions start

    void Scene::addGameObject(GameObject &gameObject) {
        gameObjects.push_back(gameObject);
    }

    void Scene::start() {
        for (auto &obj : gameObjects) {
            obj.start();
        }
    }

    void Scene::update() {
        for (auto &obj : gameObjects) {
            obj.update();
        }
    }

    // Scene definitions end
}
