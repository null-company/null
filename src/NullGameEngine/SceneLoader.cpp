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
        boxObject->getSprite().setScale(0.25f, 0.25f);
        boxObject->setIsVisible(true);

        auto groundObject = std::make_unique<GameObject>();
        auto& groundSprite = groundObject->getSprite();
        groundSprite.setTexture(*boxTexture);
        groundSprite.setScale(2.0f, 0.35f);
        groundSprite.setPosition(0.0f, 500.0f);
        groundObject->setIsVisible(true);

        boxObject->addScript<ExampleClockedScript>(*boxObject);

        newScene->addGameObject(move(nullGameLogo));
        newScene->addGameObject(move(boxObject));
        newScene->addGameObject(move(groundObject));

        MainLoop::provideScene(move(newScene));
    };

}

