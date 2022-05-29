//
// Created by artemonchelus on 28.05.22.
//

#include "PlayerProgress/HealthBarHolder.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "plog/Log.h"
#include "PlayerAnimation.hpp"
#include "PlayerProgress/HealthBar.hpp"
#include "deque"
#include "string"
#include "ResourceManager.hpp"
#include "Graphic/Vector.hpp"

namespace null {

    void HealthBarHolder::start() {
        Component::start();
    }

    void HealthBarHolder::update() {
        Component::update();
        //order is important
        updateBars();
        checkNewPlayer();
    }

    void HealthBarHolder::checkNewPlayer() {
        auto players = gameObject.getSceneForce().findAllByTag("player");
        for (auto player: players) {
            if (!player.lock()) {
                PLOGW << "Expected player but get not valide pointer";
            }
            if (!player.lock()->getScript<PlayerAnimation>()) {
                PLOGW << "GameObject with tag player does not have player animation script";
            }
            auto playerScript = player.lock()->getScript<PlayerAnimation>();
            if (!isPlayerTagged(playerScript->name)) {
                nameToPlayer.insert(std::pair{playerScript->name, player});
                gameObject.addChild(std::move(initHealthBar(playerScript->name)));
            }
        }
    }

    void HealthBarHolder::updateBars() {
        for (auto& [name, bar]: nameToBar) {
            auto player = nameToPlayer[name].lock();
            if (player) {
                auto playerScript = player->getScript<PlayerAnimation>();
                bar->getScript<HealthBar>()->setProgress(playerScript->health / 100.f);
            }
        }
    }

    bool HealthBarHolder::isPlayerTagged(const std::string& name) {
        if (nameToBar.contains(name)) {
            assert(nameToPlayer.contains(name));
            return true;
        }
        return false;
    }

    std::shared_ptr<GameObject> HealthBarHolder::initHealthBar(const std::string& name) {
        auto healthBar = std::make_shared<GameObject>();
        nameToBar[name] = healthBar;
        healthBar->addScript<HealthBar>(*healthBar);
        healthBar->getSprite().setTexture(*ResourceManager::loadTexture(names.back()), true);
        names.insert(names.begin(), names.back());
        names.pop_back();
        auto scale = getScaleToFitNewSize<float>(sf::Vector2<float>(healthBar->getSprite().getTexture()->getSize()),
                                                 size);
        healthBar->getSprite().setScale(scale);
        healthBar->getSprite().setPosition(sf::Vector2f{(float) padding, (size.y + padding) * (nameToBar.size() - 1) + padding});
        healthBar->renderLayer = serial::FOREGROUND1;
        healthBar->visible = true;
        return healthBar;
    }

    HealthBarHolder::HealthBarHolder(GameObject& object) : Component(object) {}

    void HealthBarHolder::serialize(google::protobuf::Message& message) const {
        Component::serialize(message);
    }

    std::unique_ptr<Script> HealthBarHolder::deserialize(const google::protobuf::Message& message) {
        return std::unique_ptr<Script>();
    }
} // null