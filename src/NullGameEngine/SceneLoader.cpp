#include <memory>

#include <box2d/box2d.h>

#include <Scene.hpp>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <Scripts.hpp>
#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include <PlayerAnimation.hpp>
#include <Utility.hpp>
#include "MapManager/MapManager.hpp"

namespace null {

    // todo this is a dummy implementation
    // later reimplement this by loading stuff from file 
    // and using a resource manager
    void SceneLoader::loadSceneFromFile(std::filesystem::path) {

        // todo this should be done in a scene file
        auto newScene = std::make_shared<Scene>();
        auto& box2dWorld = newScene->getBox2dWorld();

        newScene->camera.addScript<ExampleCameraScript>(newScene->camera);

        // this texture is not released on purpose, because it MUST exist for as long
        // as the sprite lives. todo manage it with resource manager
        sf::Texture* stadiumTexture = ResourceManager::loadTexture("background.png");
        auto backgroundGO = std::make_shared<GameObject>();
        backgroundGO->getSprite().setScale(6, 6);
//        std::cout << backgroundGO.getScript
        backgroundGO->getSprite().setTexture(*stadiumTexture);
//        backgroundGO->makeStatic(box2dWorld);
        backgroundGO->renderLayer = BACKGROUND;
        backgroundGO->visible = true;


        auto cursorObject = std::make_shared<GameObject>();

        auto spriteSheet = SpriteSheet("cursorAnim.png", sf::Vector2i(16, 16), {{"cursorAnim", 0, 0, 5}});
        cursorObject->addScript<CursorAnimation>(*cursorObject, spriteSheet);
        cursorObject->getSprite().setScale(4.0f, 4.0f);
        cursorObject->renderLayer = FOREGROUND;
        cursorObject->visible = true;

        auto player = std::make_unique<GameObject>();
        player->getSprite().setTextureRect({0, 0, 30, 54});
        player->getSprite().setScale(3.0f, 3.0f);
//        player->setPosition(300, 300);
        player->visible = true;
        player->renderLayer = FOREGROUND1;
        player->makeDynamic(box2dWorld);
        player->getRigidBody()->SetFixedRotation(true);
        newScene->camera.getScript<ExampleCameraScript>()->setTrackedGameObject(*player);
        newScene->camera.getScript<ExampleCameraScript>()->setMap(*backgroundGO);

        auto playerSpriteSheet = SpriteSheet("playerAnim.png", {30, 54}, {{"walkRight", 0, 0, 3},
                                                                          {"walkLeft",  1, 0, 3}});

        auto shape1 = new b2PolygonShape();
        auto sizeVector = Utility::pixelToMetersVector(sf::Vector2i{60, 162});
        shape1->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(), 0.0f);
        b2FixtureDef fixtureDef1;
        fixtureDef1.shape = shape1;
        fixtureDef1.density = 1;

        auto shape2 = new b2PolygonShape();
        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{90, 162});
        shape2->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(), 0.0f);
        b2FixtureDef fixtureDef2;
        fixtureDef2.shape = shape2;
        fixtureDef2.density = 1;

        player->addScript<PlayerAnimation>(*player, playerSpriteSheet,
                                           std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>{
                                                   {"walkRight", {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}},
                                                   {"walkLeft",  {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}}
                                           });

        MapManager mapManager(box2dWorld);
        // nonsensical actions to demonstrate
        // child adding process
        newScene->addRootGameObject(std::move(mapManager.makeBorder(backgroundGO->getSprite())));
        newScene->addRootGameObject(std::move(cursorObject));
        newScene->addRootGameObject(std::move(player));
        newScene->addRootGameObject(std::move(backgroundGO));

        MainLoop::provideScene(move(newScene));
    };

    void SceneLoader::changeScene(std::filesystem::path path) {
        loadSceneFromFile(path);
        throw SceneChangedException();
    }

}

