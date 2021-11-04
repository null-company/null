#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>

namespace null {

    class MainLoop {
        private:
            static std::unique_ptr<Scene> scene;
            MainLoop();
            static void provideScene(std::unique_ptr<Scene> newScene) {
                MainLoop::scene = std::move(newScene);
            }
        public:
            static int run();

        friend SceneLoader;
    };

    class SceneChangedException : public std::exception {  };

}

