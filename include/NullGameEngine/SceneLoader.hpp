#include <filesystem>

#include <NullGameEngine.hpp>
#include <RenderLayer.hpp>

namespace null {

    class SceneLoader {
        private:
            SceneLoader();

        public:
            static void loadSceneFromFile(std::filesystem::path);

    };

}

