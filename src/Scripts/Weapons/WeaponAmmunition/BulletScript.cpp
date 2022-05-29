#include "Component.hpp"
#include "Weapons/WeaponAmmunition/BulletScript.hpp"
#include "GameObject.hpp"
#include "Serializer.hpp"
#include "ResourceManager.hpp"
#include "PlayerAnimation.hpp"

namespace null {
    void null::BulletScript::start() {
        Component::start();
        gameObject.getSprite().setTexture(*ResourceManager::loadTexture("weapons/bullet.png"));
        gameObject.getSprite().setScale({0.010, 0.010});
        gameObject.makeDynamic();
        gameObject.renderLayer = serial::FOREGROUND3;
        gameObject.getRigidBody()->SetGravityScale(0);
        gameObject.getRigidBody()->SetLinearVelocity(
                {cos(2 * 3.14f / 360 * rotation) * speed, sin(2 * 3.14f / 360 * rotation) * speed});
        gameObject.setPosition(from);
        gameObject.visible = true;
    }

    void null::BulletScript::update() {
        Component::update();
        auto players = gameObject.getContactedGameObjects<PlayerAnimation>();
        for (auto player: players) {
            player->damage(5);
        }
        if (contactedGameObject) {
            gameObject.destroy();
        }
    }

    BulletScript::BulletScript(GameObject& object, sf::Vector2f from, float rotation, float speed)
            : Component(object),
              from(from),
              rotation(rotation),
              speed(speed) {
    }

    void BulletScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);

        auto s_script = msg.mutable_bullet_script();
        s_script->mutable_from()->set_x(from.x);
        s_script->mutable_from()->set_y(from.y);
        s_script->set_speed(speed);
        s_script->set_rotation(rotation);
    }

    std::unique_ptr<BulletScript> BulletScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto const& s_script = msg.bullet_script();

        auto p_script = std::make_unique<BulletScript>(
                *Serializer::currentDeserializationGameObject,
                sf::Vector2f{s_script.from().x(), s_script.from().y()},
                s_script.rotation(),
                s_script.speed()
        );
        p_script->guid = msg.guid();
        return p_script;
    }
}
