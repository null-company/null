#include "Weapons/WeaponHolders/WeaponStorage.hpp"
#include "Network/NetworkManagerServerScript.hpp"

#include <utility>

#include <Scene.hpp>
#include <GameObject.hpp>
#include <MainLoop.hpp>

namespace null {
    int WeaponStorage::counter = 0;

    void WeaponStorage::start() {
        Component::start();
        if (!weapons.empty()) {
            auto weapon = weapons.front();
            gameObject.getParent().lock()->addChild(std::move(weapon));
        }
        gameObject.setPosition(gameObject.getParent().lock()->getPosition());
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

    void WeaponStorage::update() {
        Component::update();
        gameObject.setPosition(gameObject.getParent().lock()->getPosition());
        if (host == Client) {
            if (isControlledByCurrentPlayer()) {
                if (gameObject.getSceneForce().windowMetaInfo.getKeyEvent().code == sf::Keyboard::E) {
                    swapWeapon();
                    sendSwapCommand();
                }
            } else {
                getStateFromNetAndApply();
            }
        } else {
            // host == Server
            processClientCommands();
        }
    }

    WeaponStorage::WeaponStorage(null::GameObject& object,
                                 const std::vector<std::shared_ptr<GameObject>>& weapons)
            : Component(object) {
        gameObject.addTag("WeaponStorage");
        for (auto& weapon: weapons) {
            addWeapon(weapon);
        }
    }

    void WeaponStorage::addWeapon(const std::shared_ptr<GameObject>& weapon) {
        auto tag = "WeaponInStorage:" + std::to_string(WeaponStorage::counter++);
        weapon->addTag(tag);
        this->weapons.push(weapon);
        weaponToTag.insert(std::pair{weapon, tag});
    }

    void WeaponStorage::swapWeapon() {
        if (weapons.empty()) {
            return;
        }
        auto weapon = gameObject.getParent().lock()->findFirstChildrenByTag(weaponToTag[weapons.front()]);
        if (weapon != nullptr) {
            gameObject.getParent().lock()->deleteChild(&*weapon);
            weapons.push(weapon);
            weapons.pop();
        }
        auto newWeapon = weapons.front();
        gameObject.getParent().lock()->addChild(std::move(newWeapon));
    }

    void WeaponStorage::sendSwapCommand() {
        net::GameMessage::ClientCommand commandMessage;
        commandMessage.set_subscriber_id(gameObject.guid);
        bool swap = true;
        *commandMessage.mutable_content() = CommandConverter::makeMessageFrom(
                swap
        );
        networkManagerScript->getNetworkManager().sendCommandToServer(commandMessage);
    }

    void WeaponStorage::getStateFromNetAndApply() {
        clientQueue.processMessageIfAny([this](net::GameMessage::SubscriberState& receivedState) {
            bool swap;
            PrimitiveStateConverter::restoreFromMessage(
                    receivedState.content(),
                    swap
            );
            if (swap) {
                swapWeapon();
            }
        });
    }

    void WeaponStorage::processClientCommands() {
        serverQueue.processMessageIfAny([this](net::GameMessage::ClientCommand& clientCommand) {
            bool swap;
            PrimitiveStateConverter::restoreFromMessage(
                    clientCommand.content(),
                    swap
            );
            if (swap) {
                swapWeapon();
            }

            net::GameMessage::SubscriberState stateToBroadcast;
            stateToBroadcast.set_subscriber_id(gameObject.guid);
            *stateToBroadcast.mutable_content() = CommandConverter::makeMessageFrom(
                    swap
            );
            MainLoop::serverArbiter->getGameServer().broadcastMessage(
                    stateToBroadcast
            );
        });
    }
}
