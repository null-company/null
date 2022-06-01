//
// Created by artemonchelus on 01.06.22.
//

#pragma once

#include "Animation.hpp"
#include <Schedulers/Timer.hpp>

namespace null {
    class BackgroundAnimation : public Animation {
    private:
        std::string animation;
        Timer timer{std::chrono::milliseconds(40)};
    public:
        void update() override;

        void serialize(google::protobuf::Message& message) const override;

        BackgroundAnimation(GameObject& object, const SpriteSheet& sheet);

        ~BackgroundAnimation() override = default;

        void start() override;
    };
} // null
