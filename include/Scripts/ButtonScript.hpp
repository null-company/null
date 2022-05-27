#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include <Script.hpp>
#include <GameObject.hpp>
#include <Scripts.hpp>
#include <box2d/box2d.h>

namespace null {

    class ButtonScript : public Script {
        private:
            sf::Texture unpressedButtonTexture;
            sf::Texture pressedButtonTexture;
            GameObject* cursor;
            b2Body* rigidBody;
            sf::Sprite* sprite;
            std::function<void()> callback;
        public:

            void start() override;
            void update() override;

            void press();

            ButtonScript(GameObject& go, sf::Texture unpressedButtonTexture,
                    sf::Texture pressedButtonTexture, const std::function<void()> callback);

            void serialize(google::protobuf::Message &) const override {};
            static std::unique_ptr<Component> deserialize(const google::protobuf::Message&) {
                return {};
            };
    };

}

