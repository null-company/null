#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>

namespace null {

    class MainLoop {
        private:
            std::unique_ptr<Scene> scene;
        
        public:
            explicit MainLoop(std::unique_ptr<Scene> scene):
                scene(std::move(scene)) {  }
            void run();
    };

    class SceneChangedException : public std::exception {  };

}

