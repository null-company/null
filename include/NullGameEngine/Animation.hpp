#pragma once

#include <NullGameEngine.hpp>
#include <Component.hpp>
#include "SpriteSheet.hpp"

namespace null{
    // Represents a Component which changes GameObject's sprite via a SpriteSheet
    class Animation : public Component {
    public:
        SpriteSheet spriteSheet;
        void update() override;
        explicit Animation(GameObject&, SpriteSheet const&);

        virtual void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };

}
