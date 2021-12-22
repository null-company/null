#pragma once

#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>

namespace null {

    class MainLoop {
        private:
            static std::shared_ptr<Scene> scene;
            static sf::Window* window;
            MainLoop();
            static void provideScene(std::shared_ptr<Scene> newScene) {
                MainLoop::scene = newScene;
            }
        public:
            static int run();
            static sf::Window& getWindow();

        friend SceneLoader;
    };

    class SceneChangedException : public std::exception {  };

}

