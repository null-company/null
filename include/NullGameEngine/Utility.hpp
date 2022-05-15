#pragma once

#include <type_traits>
#include <random>

#include <box2d/b2_math.h>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include "scene.pb.h"
#include "Script.hpp"
#include "Component.hpp"
#include <Scripts.hpp>

namespace null::Utility {

    constexpr int meterToPixel = 100;
    constexpr float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);

    template<typename T>
    b2Vec2 pixelToMetersVector(const sf::Vector2<T>& pixelVec) {
        const b2Vec2 metersVec(pixelVec.x * pixelToMeter, pixelVec.y * pixelToMeter);
        return metersVec;
    }

    template<typename T>
    sf::Vector2<T> meterToPixelVector(const b2Vec2& meterVec) {
        T x = static_cast<T>(meterVec.x * meterToPixel);
        T y = static_cast<T>(meterVec.y * meterToPixel);

        return sf::Vector2<T>(x, y);
    }

    namespace {
        //https://stackoverflow.com/questions/56655002/generating-a-random-uint64-t
        template <typename E>
        uint64_t guidImpl(E& engine) {
            std::uniform_int_distribution<unsigned long long> dis(
                    std::numeric_limits<std::uint64_t>::min(),
                    std::numeric_limits<std::uint64_t>::max()
            );
            return dis(engine);
        }
    }
    }

    // TODO fix targets to get rid of "inline" keyword
    inline uint64_t generateGuid() {
        std::random_device rd{};
        std::mt19937 gen(rd());
        return guidImpl(gen);
    }

    const std::unordered_map<serial::Script::ScriptInstanceCase, std::function<std::unique_ptr<Script>(const google::protobuf::Message&)>>
            scriptSerializationMap = {
            {serial::Script::ScriptInstanceCase::kPlayerAnimation, PlayerAnimation::deserialize},
            {serial::Script::ScriptInstanceCase::kExampleCameraScript, ExampleCameraScript::deserialize},
            {serial::Script::ScriptInstanceCase::kExampleClockedScript, ExampleClockedScript::deserialize},
            {serial::Script::ScriptInstanceCase::kReloadSceneScript, ReloadSceneScript::deserialize},
            {serial::Script::ScriptInstanceCase::kCameraScript, CameraScript::deserialize},
            {serial::Script::ScriptInstanceCase::kButtonScript, ButtonScript::deserialize},
            {serial::Script::ScriptInstanceCase::kExampleScript, ExampleScript::deserialize},
            {serial::Script::ScriptInstanceCase::kStraightWeaponScript, StraightWeaponScript::deserialize},
            {serial::Script::ScriptInstanceCase::kBulletScript, BulletScript::deserialize},
            {serial::Script::ScriptInstanceCase::kNetworkPlayerScript, NetworkPlayerScript::deserialize},
            {serial::Script::ScriptInstanceCase::kCursorAnimation, CursorAnimation::deserialize},
    };
}
