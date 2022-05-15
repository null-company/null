#pragma once

#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <utility>
#include <client/ClientNetworkManager.h>
#include <server/ServerArbiter.h>

namespace null {

    class MainLoop {
    private:
        static std::shared_ptr<Scene> scene; // TODO has to be thread_local
        static void provideScene(std::shared_ptr<Scene> newScene) {
            MainLoop::scene = std::move(newScene);
        }
    public:
        MainLoop() = delete;
        static ServerArbiter* serverArbiter;
        thread_local static bool attachWindow;
        static int run();

        friend SceneLoader;
    };

    class SceneChangedException : public std::exception {};

}
