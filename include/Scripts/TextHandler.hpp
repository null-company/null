//
// Created by artemonchelus on 30.05.22.
//

#pragma once

#include <Script.hpp>

namespace null {

    class TextHandler : public Script {
    public:
        bool active = false;
        bool wasStarted = false;

        TextHandler(GameObject& object);

        void start() override;

        void update() override;

        void serialize(google::protobuf::Message& message) const override;
    };

} // null
