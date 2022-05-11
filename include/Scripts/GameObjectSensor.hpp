#pragma once

#include <memory>
#include "Script.hpp"
#include "Schedulers/Timer.hpp"
#include "SFML/System/Vector2.hpp"
#include "Weapons/WeaponHolders/WeaponStorage.hpp"
#include <functional>
#include <GameObject.hpp>
#include <PlayerAnimation.hpp>

namespace null {
    class GameObjectSensor : public Script {
    private:
        Timer timer{std::chrono::milliseconds(10000)};
        sf::Vector2f bias;
        std::shared_ptr<GameObject> objectToAdd;
    public:
        const std::function<void(GameObject* touchedGameObject,
                                 GameObject* sensor)> touchHandler;

        GameObjectSensor(GameObject& object, const sf::Vector2f& bias, std::shared_ptr<GameObject> objectToAdd,
                         std::function<void(GameObject* touchedGameObject,
                                            GameObject* sensor)> onTouch, const Timer& timer = {std::chrono::milliseconds(5000)});

        void start() override;

        std::shared_ptr<GameObject> getObjectToAdd() const;

        void onTouch(GameObject* touchedObject);

        void update() override;
    };

    void onPlayerEnter(GameObject* touchedObject, GameObject* sensor);
}
