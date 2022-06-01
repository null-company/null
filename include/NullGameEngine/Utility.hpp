#pragma once

#include <type_traits>
#include <random>

#include <box2d/b2_math.h>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include "scene.pb.h"
#include <Script.hpp>
#include <Scene.hpp>
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
        auto x = static_cast<T>(meterVec.x * meterToPixel);
        auto y = static_cast<T>(meterVec.y * meterToPixel);

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

    // TODO fix targets to get rid of "inline" keyword
    inline uint64_t generateGuid() {
        std::random_device rd{};
        std::mt19937 gen(rd());
        return guidImpl(gen);
    }

    inline std::string gameObjectToFilename(GameObject const* go, bool json) {
        return go->getScene().lock()->getName() + "/" + (json ? "json" : "pbuf") + "/gameObjects/" + std::to_string(go->guid) + (json ? ".json" : ".pbuf");
    }
}
