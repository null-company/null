
#pragma once

#include "Script.hpp"

namespace null {

    class Wall: public Script{
    public:
        Wall(GameObject& object);

        void start() override;

        void update() override;

        void serialize(google::protobuf::Message& message) const override;

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message& message);
    };

}
