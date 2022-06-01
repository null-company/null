#include "Weapons/WeaponHolders/StraightWeaponScript.hpp"
#include "Script.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "box2d/box2d.h"
#include "Serializer.hpp"
#include "Weapons/WeaponAmmunition/BulletScript.hpp"
#include "Graphic/Vector.hpp"
#include "PlayerAnimation.hpp"
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <Network/NetworkManagerServerScript.hpp>

namespace null {
    void StraightWeaponScript::start() {
        WeaponScript::start();
        initialScale = gameObject.getSprite().getScale();
        gunShotSound = &ResourceManager::getSound("gunshot.ogg");
        gameObject.getSprite().setOrigin(sf::Vector2f(40, 60));

        host = MainLoop::serverArbiter == nullptr ? Client : Server;
        if (host == Client) {
            auto networkManagerObject =
                    gameObject.getScene().lock()->findFirstByTag("network-manager");
            networkManagerScript = networkManagerObject.lock()->getScript<NetworkManagerClientScript>();
            clientQueue.attachTo(&networkManagerScript->getNetworkManager().subscribe(gameObject.guid));
        } else {
            serverQueue.attachTo(
                    &gameObject.getScene().lock()->findFirstByTag("network-manager")
                            .lock()->getScript<NetworkManagerServerScript>()->subscribe(gameObject.guid)
            );
        }
    }

    void StraightWeaponScript::update() {
        WeaponScript::update();
        auto scene = gameObject.getScene().lock();
        if (scene == nullptr) {
            return;
        }

        auto parent = gameObject.getParent().lock();
        if (parent == nullptr) {
            return;
        }
        if (host == Client) {
            if (isControlledByCurrentPlayer()) {
                auto mouseCoords = scene->getWindowMetaInfo().absoluteMouseWorldCoords;
                sf::Vector2f weaponDirection = mouseCoords - gameObject.getPosition();
                weaponDirection = normalize(weaponDirection);
                weaponDirection *= 120.0f;
                auto weaponEndPos = gameObject.getPosition() + weaponDirection;
                bool isShooting = false;
                setWeaponRotationByMouseCoords(mouseCoords);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && checkIfCanShoot()) {
                    shoot(weaponEndPos, mouseCoords);
                    isShooting = true;
                }
                sendState(mouseCoords, weaponEndPos, isShooting);
            } else {
                getStateFromNetAndApply();
            }
        } else {
            // host == Server
            processClientCommands();
        }
        auto coords = parent->getPosition() + sf::Vector2f(60, 60);
        gameObject.setPosition(coords);
    }

    void StraightWeaponScript::setWeaponRotationByMouseCoords(sf::Vector2<float> mouseCoords) {
        auto scene = gameObject.getScene().lock();
        if (scene == nullptr) {
            return;
        }
        sf::Vector2f weaponDirection = mouseCoords - gameObject.getPosition();
        gameObject.getSprite().setRotation(getAngle(weaponDirection));
        if (getAngle(weaponDirection) > 90 and getAngle(weaponDirection) < 270) {
            gameObject.getSprite().setScale(initialScale.x, -1.0f * initialScale.y);
        } else {
            gameObject.getSprite().setScale(initialScale.x, initialScale.y);
        }
    }

    void StraightWeaponScript::shoot(sf::Vector2f from, sf::Vector2f to) {
        saveShotInfo();
        static int i = 0;
        auto bullet = std::make_shared<GameObject>();
        bullet->setName(std::string("Bullet: ") + std::to_string(i++));
        bullet->addScript<BulletScript>(*bullet, from, gameObject.getSprite().getRotation() + d(gen), speed);

        gameObject.addChild(std::move(bullet));
        gunShotSound->play();
    }

    StraightWeaponScript::StraightWeaponScript(GameObject& object, double deviance) : WeaponScript(object) {
        d = std::normal_distribution<>(0, deviance);
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapon.png");
//        gameObject.getSprite().setOrigin(sf::Vector2f(250, 100));
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.40, 0.25);
        speed = 8;
        gameObject.renderLayer = serial::FOREGROUND2;
        gameObject.visible = true;
    }

    void StraightWeaponScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);

        auto s_script = msg.mutable_straight_weapon_script();
        s_script->set_deviance(d.stddev());
    }

    std::unique_ptr<Component> StraightWeaponScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto p_script = std::make_unique<StraightWeaponScript>(
                *Serializer::currentDeserializationGameObject,
                msg.straight_weapon_script().deviance()
        );
        return p_script;
    }

    void StraightWeaponScript::sendState(sf::Vector2f mousePos, sf::Vector2f weaponEnd, bool isShooting) {
        net::GameMessage::ClientCommand commandMessage;
        commandMessage.set_subscriber_id(gameObject.guid);
        *commandMessage.mutable_content() = CommandConverter::makeMessageFrom(
                mousePos.x, mousePos.y,
                weaponEnd.x, weaponEnd.y,
                isShooting
        );
        networkManagerScript->getNetworkManager().sendCommandToServer(commandMessage);
    }

    void StraightWeaponScript::getStateFromNetAndApply() {
        clientQueue.processMessageIfAny([this](net::GameMessage::SubscriberState& receivedState) {
            sf::Vector2f mousePos;
            sf::Vector2f weaponEnd;
            bool isShooting;
            PrimitiveStateConverter::restoreFromMessage(
                    receivedState.content(),
                    mousePos.x, mousePos.y,
                    weaponEnd.x, weaponEnd.y,
                    isShooting
            );
            setWeaponRotationByMouseCoords(mousePos);
            if (isShooting) {
                shoot(weaponEnd, mousePos);
            }
        });
    }

    void StraightWeaponScript::processClientCommands() {
        serverQueue.processMessageIfAny([this](net::GameMessage::ClientCommand& clientCommand) {
            sf::Vector2f mousePos;
            sf::Vector2f weaponEnd;
            bool isShooting;
            PrimitiveStateConverter::restoreFromMessage(
                    clientCommand.content(),
                    mousePos.x, mousePos.y,
                    weaponEnd.x, weaponEnd.y,
                    isShooting
            );
            setWeaponRotationByMouseCoords(mousePos);
            if (isShooting) {
                shoot(weaponEnd, mousePos);
            }

            net::GameMessage::SubscriberState stateToBroadcast;
            stateToBroadcast.set_subscriber_id(gameObject.guid);
            *stateToBroadcast.mutable_content() = CommandConverter::makeMessageFrom(
                    mousePos.x, mousePos.y,
                    weaponEnd.x, weaponEnd.y,
                    isShooting
            );
            MainLoop::serverArbiter->getGameServer().broadcastMessage(
                    stateToBroadcast
            );
        });
    }
}
