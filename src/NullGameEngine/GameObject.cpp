#include <memory>

#include <GameObject.hpp>
#include <Script.hpp>

namespace null {
    GameObject::GameObject():
    isVisible(false) {
        this->children = std::vector<std::shared_ptr<GameObject>>();
        this->tags = std::set<std::string>();
        this->sprite = sf::Sprite();
    }

    sf::Sprite& GameObject::getSprite() {
        return this->sprite;
    }

    bool GameObject::getIsVisible() {
        return this->isVisible;
    }

    void GameObject::setIsVisible(bool isVisible){
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
        return std::weak_ptr<GameObject>(this->children[index]);
    }

    void GameObject::addChild(const std::shared_ptr<GameObject> &child) {
        children.push_back(child);
    }

    // todo concern pointer leakage
    std::vector<std::unique_ptr<Script>> &GameObject::getScripts() {
        return scripts;
    }

    void GameObject::addScript(std::unique_ptr<Script> script) {
        scripts.push_back(std::move(script));
    }

    // todo cant put this code here, duplicates the header
    /* template<class T> */
    /* void GameObject::addScript(GameObject& go) { */
    /*     std::unique_ptr<Script> script = std::make_unique<T>(go); */
    /*     scripts.push_back(std::move(script)); */
    /* } */

    void GameObject::addTag(const std::string &str) {
        tags.insert(str);
    }

    bool GameObject::removeTag(const std::string &str) {
        auto result = !tags.find(str)->empty();
        tags.erase(str);
        return result;
    }

    const sf::Transform& GameObject::getTransform() {
        return sprite.getTransform();
    }

    const sf::Vector2f& GameObject::getPosition() {
        return sprite.getPosition();
    }

    void GameObject::setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void GameObject::setPosition(sf::Vector2f &pos) {
        sprite.setPosition(pos);
    }

    void GameObject::start() {
        for (auto &script : scripts) {
            script->start();
        }
    }

    void GameObject::update() {
        for (auto &script : scripts) {
            script->update();
        }
    }

}
