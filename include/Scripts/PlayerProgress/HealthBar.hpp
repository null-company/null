#pragma once

#include "Script.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace null {
    class HealthBar : public Script {
    private:
        sf::IntRect initialRect;
        sf::Vector2f initialPosition;
    public:
        void start() override;

        void update() override;

        void setProgress(float progress);

        HealthBar(GameObject& object);

        void serialize(google::protobuf::Message& message) const override;
    };
}
