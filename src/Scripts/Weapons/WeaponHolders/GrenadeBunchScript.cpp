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
#include "Network/NetworkManagerServerScript.hpp"
#include <MainLoop.hpp>

namespace null {
    void GrenadeBunchScript::start() {
        WeaponScript::start();
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

    void GrenadeBunchScript::update() {
        WeaponScript::update();
        gameObject.setPosition(gameObject.getParent().lock()->getPosition() + sf::Vector2f(10, 30));

        if (host == Client) {
            if (isControlledByCurrentPlayer()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !scheduler.isStarted() && checkIfCanShoot()) {
                    scheduler.start();
                    return;
                }

                bool isShooting = false;
                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && scheduler.isStarted()) {
                    auto& scene = gameObject.getSceneForce();
                    scheduler.end();
                    shoot(gameObject.getPosition(), scene.getWindowMetaInfo().absoluteMouseWorldCoords);
                    isShooting = true;
                }
                auto& scene = gameObject.getSceneForce();
                auto mouseCoords = scene.getWindowMetaInfo().absoluteMouseWorldCoords;
                if (isShooting) {
                    sendState(mouseCoords, isShooting);
                }
            } else {
                getStateFromNetAndApply();
            }
        } else {
            // host == Server
            processClientCommands();
        }
    }

    GrenadeBunchScript::GrenadeBunchScript(GameObject& object) : WeaponScript(object) {
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapons/grenade.png");
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.3, 0.3);
        gameObject.renderLayer = serial::FOREGROUND2;
        gameObject.visible = true;
        restartTime = std::chrono::milliseconds(500);
    }

    void GrenadeBunchScript::shoot(sf::Vector2f from, sf::Vector2f to) {
        saveShotInfo();
        auto& scene = gameObject.getSceneForce();
        auto speed = scheduler.getValue();
        auto grenade = std::make_shared<GameObject>();
        grenade->getSprite().setTextureRect({0, 0, 64, 64});
        grenade->getSprite().setScale({3, 3});
        grenade->makeDynamic(gameObject.getSceneForce().getBox2dWorld());
        grenade->renderLayer = serial::FOREGROUND;
        grenade->visible = true;

        auto grenadeSpriteSheet = SpriteSheet("weapons/grenade_anim.png", sf::Vector2i{64, 64}, {{"", 0, 0, 12}});

        //TODO Memory leak......
        auto shape1 = new b2PolygonShape();
        auto sizeVector = Utility::pixelToMetersVector(sf::Vector2i{15, 25});
        shape1->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, grenade->getRigidBody()->GetLocalCenter(), 0.0f);
        b2FixtureDef fixtureDef1;
        fixtureDef1.shape = shape1;
        fixtureDef1.density = 1;
        fixtureDef1.restitution = 0.6;
        auto b2Filter = fixtureDef1.filter;
        b2Filter.categoryBits = GRENADE_CATEGORY;
        b2Filter.maskBits = ALL_CATEGORIES & ~PLAYER_CATEGORY;
        fixtureDef1.filter = b2Filter;

        // Волшебные константы при SetAsBox но нужно решить
        auto explodeSizeVector = Utility::pixelToMetersVector(sf::Vector2i{64, 64});
        auto shape2 = new b2PolygonShape();
        shape2->SetAsBox(explodeSizeVector.x*1.3, explodeSizeVector.y*1.3, grenade->getRigidBody()->GetLocalCenter(), 0.0f);
        b2FixtureDef fixtureDef2;
        fixtureDef2.shape = shape2;
        fixtureDef2.isSensor = true;
        grenade->addScript<GrenadeScript>(*grenade,
                                          grenadeSpriteSheet,
                                          CollisionMap(
                                                  {{"", {{fixtureDef1}, {fixtureDef1}, {fixtureDef1}, {fixtureDef1}, {fixtureDef1},
                                                         {fixtureDef1}, {fixtureDef1}, {fixtureDef1}, {fixtureDef2},
                                                         {fixtureDef2}, {fixtureDef2}, {fixtureDef2}, {fixtureDef2}}}}),
                                          speed,
                                          getAngle(to - from),
                                          gameObject.getPosition() - sf::Vector2f{0, 60});
        gameObject.addChild(std::move(grenade));
    }

    void GrenadeBunchScript::processClientCommands() {
        serverQueue.processMessageIfAny([this](net::GameMessage::ClientCommand& clientCommand) {
            sf::Vector2f mousePos;
            bool isShooting;
            PrimitiveStateConverter::restoreFromMessage(
                    clientCommand.content(),
                    mousePos.x, mousePos.y,
                    isShooting
            );
            if (isShooting) {
                auto& goPos = gameObject.getPosition();
                shoot(goPos, mousePos);
            }

            net::GameMessage::SubscriberState stateToBroadcast;
            stateToBroadcast.set_subscriber_id(gameObject.guid);
            *stateToBroadcast.mutable_content() = CommandConverter::makeMessageFrom(
                    mousePos.x, mousePos.y,
                    isShooting
            );
            MainLoop::serverArbiter->getGameServer().broadcastMessage(
                    stateToBroadcast
            );
        });
    }

    void GrenadeBunchScript::sendState(sf::Vector2<float> mousePosition, bool isShooting) {
        net::GameMessage::ClientCommand commandMessage;
        commandMessage.set_subscriber_id(gameObject.guid);
        *commandMessage.mutable_content() = CommandConverter::makeMessageFrom(
                mousePosition.x, mousePosition.y,
                isShooting
        );
        networkManagerScript->getNetworkManager().sendCommandToServer(commandMessage);
    }

    void GrenadeBunchScript::getStateFromNetAndApply() {
        clientQueue.processMessageIfAny([this](net::GameMessage::SubscriberState& receivedState) {
            sf::Vector2f mousePos;
            bool isShooting;
            PrimitiveStateConverter::restoreFromMessage(
                    receivedState.content(),
                    mousePos.x, mousePos.y,
                    isShooting
            );
            auto& goPos = gameObject.getPosition();
            if (isShooting) {
                shoot(goPos, mousePos);
            }
        });
    }
}
