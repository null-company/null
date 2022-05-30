//
// Created by artemonchelus on 27.04.22.
//

#include "Weapons/WeaponHolders/GrenadeBunchScript.hpp"
#include "ResourceManager.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Graphic/Vector.hpp"
#include "Weapons/WeaponAmmunition/GrenadeScript.hpp"
#include "Utility.hpp"
#include "Physics/CollisionCategories.hpp"
#include "SFML/Graphics.hpp"
#include "Serializer.hpp"

namespace null {
    void GrenadeBunchScript::start() {
        Component::start();

    }

    void null::GrenadeBunchScript::update() {
        Component::update();
        gameObject.setPosition(gameObject.getParent().lock()->getPosition() + sf::Vector2f(10, 30));

        auto& scene = gameObject.getSceneForce();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !scheduler.isStarted() && checkIfCanShoot()) {
            scheduler.start();
            return;
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && scheduler.isStarted()) {
            scheduler.end();
            shoot(gameObject.getPosition(), scene.getWindowMetaInfo().absoluteMouseWorldCoords);
        }
    }

    GrenadeBunchScript::GrenadeBunchScript(GameObject& object) : WeaponScript(object) {
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapons/grenade.png");
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.3, 0.3);
        gameObject.renderLayer = serial::FOREGROUND2;
        gameObject.visible = true;
        restartTime = std::chrono::milliseconds(2000);
    }

    void GrenadeBunchScript::shoot(sf::Vector2f from, sf::Vector2f to) {
        saveShotInfo();
        auto& scene = gameObject.getSceneForce();
        auto speed = scheduler.getValue();
        auto grenade = std::make_shared<GameObject>();
        grenade->getSprite().setTextureRect({0, 0, 32, 32});
        grenade->getSprite().setScale({3, 3});
        grenade->makeDynamic(gameObject.getSceneForce().getBox2dWorld());
        grenade->renderLayer = serial::FOREGROUND;
        grenade->visible = true;

        auto grenadeSpriteSheet = SpriteSheet("weapons/grenade_anim.jpg", sf::Vector2i{32, 32}, {{"", 0, 0, 8}});

        //TODO Memory leak......
        auto shape1 = new b2PolygonShape();
        auto sizeVector = Utility::pixelToMetersVector(sf::Vector2i{30, 40});
        shape1->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, grenade->getRigidBody()->GetLocalCenter(), 0.0f);
        b2FixtureDef fixtureDef1;
        fixtureDef1.shape = shape1;
        fixtureDef1.density = 1;
        fixtureDef1.restitution = 0.6;
        auto b2Filter = fixtureDef1.filter;
        b2Filter.categoryBits = GRENADE_CATEGORY;
        b2Filter.maskBits = ALL_CATEGORIES & ~PLAYER_CATEGORY;
        fixtureDef1.filter = b2Filter;

        auto shape2 = new b2PolygonShape();
        shape2->SetAsBox(sizeVector.x * 1.5, sizeVector.y * 1.5, grenade->getRigidBody()->GetLocalCenter(), 0.0f);
        b2FixtureDef fixtureDef2;
        fixtureDef2.shape = shape2;
        fixtureDef2.isSensor = true;
        grenade->addScript<GrenadeScript>(*grenade,
                                          grenadeSpriteSheet,
                                          CollisionMap(
                                                  {{"", {{fixtureDef1}, {fixtureDef1}, {fixtureDef1}, {fixtureDef1},
                                                         {fixtureDef2},
                                                         {fixtureDef2}, {fixtureDef2}, {fixtureDef2}, {fixtureDef2}}}}),
                                          speed,
                                          getAngle(scene.getWindowMetaInfo().absoluteMouseWorldCoords -
                                                   gameObject.getPosition()),
                                          gameObject.getPosition() - sf::Vector2f{0, 60});
        gameObject.addChild(std::move(grenade));
    }

    void GrenadeBunchScript::serialize(google::protobuf::Message& message) const {
        auto& msg = (serial::Script&)message;
        msg.mutable_grenade_bunch_script();
    }

    std::unique_ptr<Script> GrenadeBunchScript::deserialize(const google::protobuf::Message& message) {
        auto p_script = std::make_unique<GrenadeBunchScript>(
                *Serializer::currentDeserializationGameObject
                );
        return p_script;
    }
}
