#include <chrono>
#include <unistd.h>
#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>

namespace null {

    void MainLoop::run() {
        unsigned int timeToWait = 1000000 * 2;
sceneStart:
        this->scene->start();

        try {
            while(true) {
                scene->update();
                usleep(timeToWait);
            }
        } 
        catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }

    }

}

