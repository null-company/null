#include <NullGameEngine.hpp>
#include <Component.hpp>
#include "SpriteSheet.hpp"

namespace null{
    class Animation : public Component, public SpriteSheet {
    public:
        void update() override;
        explicit Animation(GameObject&, SpriteSheet&);
    };

}
