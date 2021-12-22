#pragma once

#include <Animation.hpp>

namespace null {
    class CursorAnimation : public Animation {
    private:
        static constexpr auto cursorAnim = "cursorAnim";
        int frameCount = 0;
    public:
        void start() override;
        void update() override;
        explicit CursorAnimation(GameObject&, SpriteSheet);

        serial::Script prefabSerialize() override;
    };

}
