#pragma once

#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>

namespace null {

    class MainLoop {
        private:
            static std::shared_ptr<Scene> scene;
            MainLoop();
            static void provideScene(std::shared_ptr<Scene> newScene) {
                MainLoop::scene = std::move(newScene);
            }
        public:
            static int run();

        friend SceneLoader;
    };

    class SceneChangedException : public std::exception {  };

}

