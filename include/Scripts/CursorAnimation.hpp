#pragma once

#include <Animation.hpp>
#include <WindowMetaInfo.hpp>

namespace null {
    class CursorAnimation : public Animation {
    private:
        static constexpr auto cursorAnim = "cursorAnim";
        int frameCount = 0;
        WindowMetaInfo * windowMetaInfo;
    public:
        void start() override;

        void update() override;

        explicit CursorAnimation(GameObject&, SpriteSheet);

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };

}
