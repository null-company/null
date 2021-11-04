#include <string>

#include <NullGameEngine.hpp>

namespace null {

    class SceneLoader {
        private:
            SceneLoader();

        public:
            // todo this is a dummy function, make it use normal filepaths
            static void loadSceneFromFile(std::string);

    };

}


