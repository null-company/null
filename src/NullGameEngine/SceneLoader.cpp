#include <memory>
        
#include <Scene.hpp>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <Scripts.hpp>
#include <GameObject.hpp>

namespace null {

    // todo this is a dummy implementation
    // later reimplement this by loading stuff from file 
    // and using a resource manager
    void SceneLoader::loadSceneFromFile(std::string) {
        
        // todo this should be done in a scene file
        auto newScene = std::make_unique<Scene>();

        // this texture is not released on purpose, because it MUST exist for as long
        // as the sprite lives. todo manage it with resource manager
        sf::Texture* nullTexture = new sf::Texture();
        nullTexture->loadFromFile("../res/null.jpg");

        auto nullGameLogo = std::make_unique<GameObject>();
        nullGameLogo->getSprite().setTexture(*nullTexture);
        nullGameLogo->setIsVisible(true);

        auto boxTexture = new sf::Texture();
        boxTexture->loadFromFile("../res/box.png");

        auto boxObject = std::make_unique<GameObject>();
        boxObject->getSprite().setTexture(*boxTexture);
        boxObject->setIsVisible(true);

        boxObject->addScript<ExampleClockedScript>(*boxObject);

        newScene->addGameObject(move(nullGameLogo));
        newScene->addGameObject(move(boxObject));

        MainLoop::provideScene(move(newScene));
    };

}

