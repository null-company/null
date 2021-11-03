#include <NullGameEngine.hpp>
#include <GameObject.hpp>

namespace null {
    class Scene{
    private:
        GameObject rootGameObject;
        std::vector<GameObject> gameObjects;
    public:
        Scene() {
            gameObjects = std::vector<GameObject>();
            rootGameObject = GameObject();
        }

        void start();

        void update();

        void addGameObject(GameObject &gameObject);
    };
}

