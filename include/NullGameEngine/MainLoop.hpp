#pragma once

#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <client/ClientNetworkManager.h>

namespace null {

    class MainLoop {
        private:
            static std::shared_ptr<Scene> scene;
            MainLoop();
            static void provideScene(std::shared_ptr<Scene> newScene) {
                MainLoop::scene = std::move(newScene);
            }
        public:
            static ClientNetworkManager clientNetworkManager;
            static int run();

        friend SceneLoader;
    };

    class SceneChangedException : public std::exception {  };

}

