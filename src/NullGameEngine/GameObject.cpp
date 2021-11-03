#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

namespace null {

    class GameObject {

        // shared pointer
        std::vector<null::GameObject*> children;

        // weak pointer
        GameObject* parent;

        // unique or shared pointer
        std::vector<null::Script*> scripts;

        // shared or unique pointer
        sf::Sprite* sprite;

        std::vector<std::string> tags;

        bool isVisible = true;

        /* public: */
        /* GameObject(std::vector<GameObject*> children, GameO) : */
        /*     children(children), */ 
        /* {  } */

    };

}

