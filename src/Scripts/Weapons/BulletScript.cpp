
#include "Component.hpp"
#include "Weapon/BulletScript.hpp"
#include "GameObject.hpp"
#include <ResourceManager.hpp>
#include <Scene.hpp>

namespace null {
    void null::BulletScript::start() {
        Component::start();
        gameObject.getSprite().setTexture(*ResourceManager::loadTexture("weapons/bullet.png"));
        gameObject.getSprite().setScale({0.010, 0.010});
        gameObject.makeDynamic();
        gameObject.renderLayer = FOREGROUND3;
        gameObject.getRigidBody()->SetGravityScale(0);
        gameObject.getRigidBody()->SetLinearVelocity({cos(2 * 3.14f / 360 * rotation) * speed, sin(2 * 3.14f / 360 * rotation) * speed});
        gameObject.setPosition(from);
        gameObject.visible = true;
    }

    void null::BulletScript::update() {
        Component::update();
    }

    BulletScript::BulletScript(GameObject& object, sf::Vector2f from, float rotation, float speed)
            : Component(object),
              from(from),
              rotation(rotation),
              speed(speed) {
    }
}
