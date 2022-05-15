//
// Created by artemonchelus on 27.04.22.
//

#include "Weapons/WeaponHolders/GrenadeBunchScript.hpp"
#include "ResourceManager.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Graphic/Vector.hpp"
#include "Weapons/WeaponAmmunition/GrenadeScript.hpp"

namespace null {
    void GrenadeBunchScript::start() {
        Component::start();

    }

    void null::GrenadeBunchScript::update() {
        Component::update();
        gameObject.setPosition(gameObject.getParent().lock()->getPosition() + sf::Vector2f(10, 30));

        auto& scene = gameObject.getSceneForce();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !scheduler.isStarted() and checkIfCanShoot()) {
            scheduler.start();
            return;
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) and scheduler.isStarted()) {
            scheduler.end();
            shoot(gameObject.getPosition(), scene.getWindowMetaInfo().absoluteMouseWorldCoords);
        }
    }

    GrenadeBunchScript::GrenadeBunchScript(GameObject& object) : WeaponScript(object) {
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapons/grenade.png");
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.3, 0.3);
        gameObject.renderLayer = FOREGROUND2;
        gameObject.visible = true;
        restartTime = std::chrono::milliseconds(2000);
    }

    void GrenadeBunchScript::shoot(sf::Vector2f from, sf::Vector2f to) {
        saveShotInfo();
        auto& scene = gameObject.getSceneForce();
        auto speed = scheduler.getValue();
        auto grenade = std::make_shared<GameObject>();
        grenade->addScript<GrenadeScript>(*grenade, speed,
                                          getAngle(scene.getWindowMetaInfo().absoluteMouseWorldCoords -
                                                   gameObject.getPosition()),
                                          gameObject.getPosition());
        gameObject.addChild(std::move(grenade));
    }
}
