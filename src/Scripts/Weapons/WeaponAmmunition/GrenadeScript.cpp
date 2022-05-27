#include "Weapons/WeaponAmmunition/GrenadeScript.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Graphic/Vector.hpp"
#include "ResourceManager.hpp"
#include "Physics/CollisionCategories.hpp"

namespace null {
    void null::GrenadeScript::start() {
        RigidBodyAnimation::start();
        auto& scene = gameObject.getSceneForce();
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapons/grenade.png");
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.3, 0.3);
        gameObject.makeDynamic();
        gameObject.visible = true;
        gameObject.setPosition(from);

        gameObject.setCollisionCategories(GRENADE_CATEGORY);
        gameObject.setCollisionMasks(ALL_CATEGORIES & ~PLAYER_CATEGORY);
        gameObject.getRigidBody()->SetLinearVelocity(
                {cos(2 * 3.14f / 360 * angle) * speed, sin(2 * 3.14f / 360 * angle) * speed});
        gameObject.getRigidBody()->SetAngularVelocity(5);
        gameObject.getRigidBody()->GetFixtureList()->SetFriction(1);
        timer.start();
    }

    void null::GrenadeScript::update() {
        if (timer.expired()) {
            gameObject.destroy();
        }
        RigidBodyAnimation::update();
    }

    GrenadeScript::GrenadeScript(GameObject& object, float speed, float angle, sf::Vector2f from) : Component(object),
                                                                                                    speed(speed),
                                                                                                    angle(angle),
                                                                                                    from(from) {}
}
