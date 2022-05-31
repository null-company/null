#pragma once

#include <Script.hpp>

namespace null {

    class Wall: public Script {
    public:
        explicit Wall(GameObject& object);

        void start() override;

        void update() override;

        void serialize(google::protobuf::Message& message) const override;
    };

}
