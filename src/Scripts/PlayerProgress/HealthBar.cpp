//
// Created by artemonchelus on 28.05.22.
//

#include "PlayerProgress/HealthBar.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"

namespace null {
    void HealthBar::start() {
        Component::start();
        initialRect = gameObject.getSprite().getTextureRect();
        initialPosition = gameObject.getSprite().getPosition();
        auto healthBarBack = std::make_shared<GameObject>();
        healthBarBack->getSprite().setTexture(*ResourceManager::loadTexture(back), true);
        healthBarBack->getSprite().setScale(gameObject.getSprite().getScale());
        healthBarBack->addTag("health_bar_back");
        healthBarBack->renderLayer = serial::FOREGROUND2;
        healthBarBack->visible = true;
        gameObject.addChild(std::move(healthBarBack));
    }

    void HealthBar::update() {
        gameObject.setPosition(initialPosition, true);
        gameObject.findFirstChildrenByTag("health_bar_back")->setPosition(initialPosition, true);
        Component::update();
    }

    void HealthBar::serialize(google::protobuf::Message& message) const {
        Component::serialize(message);
    }

    HealthBar::HealthBar(GameObject& object, std::string backName) : Component(object), back(std::move(backName)) {

    }

    void HealthBar::setProgress(float progress) {
        auto currentRect = gameObject.getSprite().getTextureRect();
        gameObject.getSprite().setTextureRect(sf::IntRect{currentRect.left,
                                                          currentRect.top,
                                                          int(initialRect.width * progress),
                                                          currentRect.height});

    }


}
