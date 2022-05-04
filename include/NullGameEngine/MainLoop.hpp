#pragma once

#include <memory>

#include <NullGameEngine.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <client/ClientNetworkManager.h>
#include <server/ServerArbiter.h>

namespace null {

    class MainLoop {
    private:
        static std::shared_ptr<Scene> scene;
//        static sf::Window* window;
        MainLoop();
        static void provideScene(std::shared_ptr<Scene> newScene) {
            MainLoop::scene = newScene;
        }
    public:
        static bool isServer; // todo this is obviously to be refactored
        static ServerArbiter* serverArbiter;
        static GameServer* gameServer;
        static ClientNetworkManager* clientNetworkManager;
        static int run();
        static int run(bool noHead);
        static sf::Window& getWindow();

        friend SceneLoader;
    };

    class SceneChangedException : public std::exception {};

}
