#include <memory>

#include <box2d/box2d.h>
        
#include <Scene.hpp>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <Scripts.hpp>
#include <GameObject.hpp>
#include <ResourceManager.hpp>

namespace null {

    // todo this is a dummy implementation
    // later reimplement this by loading stuff from file 
    // and using a resource manager
    void SceneLoader::loadSceneFromFile(std::filesystem::path) {
        
        // todo this should be done in a scene file
        auto newScene = std::make_unique<Scene>();
        auto& box2dWorld = newScene->getBox2dWorld();

        // this texture is not released on purpose, because it MUST exist for as long
        // as the sprite lives. todo manage it with resource manager
        sf::Texture* nullTexture = ResourceManager::loadTexture("null.jpg");

        auto nullGameLogo = std::make_unique<GameObject>();
        nullGameLogo->getSprite().setTexture(*nullTexture);
        nullGameLogo->setIsVisible(true);

        auto boxTexture = ResourceManager::loadTexture("box.png");

        auto boxObject = std::make_unique<GameObject>();
        boxObject->getSprite().setTexture(*boxTexture);
        boxObject->getSprite().setScale(0.25f, 0.25f);
        boxObject->setIsVisible(true);

        auto groundObject = std::make_unique<GameObject>();
        auto& groundSprite = groundObject->getSprite();
        groundSprite.setTexture(*boxTexture);
        groundSprite.setScale(1.0f, 0.1f);
        groundSprite.setPosition(250.0f / 4, 350.0f);
        groundObject->setIsVisible(true);

        groundObject->makeStatic(box2dWorld);

        boxObject->makeDynamic(box2dWorld);
        // call this to prevent from rotation
        /* boxObject->getRigidBody()->SetFixedRotation(true); */

        boxObject->getRigidBody()->ApplyLinearImpulseToCenter(b2Vec2(15.0f, 0), true);

        newScene->addGameObject(move(nullGameLogo));
        newScene->addGameObject(move(boxObject));
        newScene->addGameObject(move(groundObject));

        MainLoop::provideScene(move(newScene));
    };

}

