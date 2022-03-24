#pragma once

#include <box2d/b2_math.h>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <GameObject.hpp>
#include <typeindex>
#include <ExampleScript.hpp>
#include <PlayerAnimation.hpp>
#include "Component.hpp"
#include "ExampleCameraScript.hpp"
#include "Weapon/StraightWeaponScript.hpp"
#include <Weapon/BulletScript.hpp>
#include <NetworkPlayerScript.hpp>

namespace null {
    class Utility {
    public:

        constexpr static int meterToPixel = 100;
        constexpr static float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);

        template<typename T>
        static b2Vec2 pixelToMetersVector(const sf::Vector2<T>& pixelVec) {
            const b2Vec2 metersVec(pixelVec.x * pixelToMeter, pixelVec.y * pixelToMeter);
            return metersVec;
        }

        template<typename T>
        static sf::Vector2<T> meterToPixelVector(const b2Vec2& meterVec) {
            T x = static_cast<T>(meterVec.x * meterToPixel);
            T y = static_cast<T>(meterVec.y * meterToPixel);

            return sf::Vector2<T>(x, y);
        }
        const static std::unordered_map<serial::Script::ScriptInstanceCase, std::function<std::unique_ptr<Script>(google::protobuf::Message*)>>
                scriptSerializationMap;
    };

}

