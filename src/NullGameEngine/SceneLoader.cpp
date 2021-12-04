#include <memory>

#include <box2d/box2d.h>

#include <Scene.hpp>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <Scripts.hpp>
#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include "google/protobuf/util/json_util.h"
#include "yaml-cpp/yaml.h"


namespace null {

    // todo this is a dummy implementation
    // later reimplement this by loading stuff from file
    // and using a resource manager
    void SceneLoader::loadSceneFromFile(std::filesystem::path) {

        // todo this should be done in a scene file
        auto newScene = std::make_shared<Scene>();
        newScene->self = newScene;
        auto &box2dWorld = newScene->getBox2dWorld();

        newScene->camera.addScript<ExampleCameraScript>(newScene->camera);

        // this texture is not released on purpose, because it MUST exist for as long
        // as the sprite lives. todo manage it with resource manager
        sf::Texture *nullTexture = ResourceManager::loadTexture("null.jpg");

        auto nullGameLogo = std::make_unique<GameObject>();
        nullGameLogo->getSprite().setTexture(*nullTexture);
        nullGameLogo->renderLayer = BACKGROUND;
        nullGameLogo->visible = true;

        auto boxTexture = ResourceManager::loadTexture("box.png");

        auto boxObject = std::make_unique<GameObject>();
        boxObject->getSprite().setTexture(*boxTexture);
        boxObject->getSprite().setScale(0.25f, 0.25f);
        boxObject->renderLayer = FOREGROUND;
        boxObject->visible = true;

        auto boxObject2 = std::make_unique<GameObject>();
        boxObject2->getSprite().setTexture(*boxTexture);
        boxObject2->getSprite().setScale(0.25f, 0.25f);
        boxObject2->setPosition(750.0f, 200.0f);
        boxObject2->getSprite().setColor(sf::Color(255U, 0U, 0U));
        boxObject2->renderLayer = BACKGROUND1;
        boxObject2->visible = true;

        auto groundObject = std::make_unique<GameObject>();
        auto &groundSprite = groundObject->getSprite();
        groundSprite.setTexture(*boxTexture);
        groundSprite.setScale(1.0f, 0.1f);
        groundSprite.setPosition(250.0f / 4, 350.0f);
        groundObject->renderLayer = FOREGROUND;
        groundObject->visible = true;

        groundObject->makeStatic(box2dWorld);

        boxObject->makeDynamic(box2dWorld);
        // call this to prevent from rotation
        /* boxObject->getRigidBody()->SetFixedRotation(true); */

        boxObject->getRigidBody()->ApplyLinearImpulseToCenter(b2Vec2(15.0f, 0), true);

        groundObject->addScript<ReloadSceneScript>(*groundObject);

        newScene->addGameObject(move(nullGameLogo));
        newScene->addGameObject(move(boxObject));
        newScene->addGameObject(move(boxObject2));
        newScene->addGameObject(move(groundObject));
        serial::Scene serialize = newScene->prefabSerialize();
        std::string message;
        google::protobuf::util::JsonOptions options;
        options.add_whitespace = true;
        google::protobuf::util::MessageToJsonString(serialize, &message, options);
        std::cout << message;
        MainLoop::provideScene(move(newScene));
    };

    void SceneLoader::changeScene(std::filesystem::path path) {
        loadSceneFromFile(path);
        throw SceneChangedException();
    }

}

