#include <memory>
#include <functional>
#include <unordered_map>

#include <box2d/box2d.h>

#include <Scene.hpp>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <Scripts.hpp>
#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include <PlayerAnimation.hpp>
#include <Utility.hpp>
#include <Serializer.hpp>
#include "MapManager/MapManager.hpp"
#include "Weapons/WeaponHolders/WeaponScript.hpp"
#include "Weapons/WeaponHolders/StraightWeaponScript.hpp"
#include "Weapons/WeaponHolders/WeaponStorage.hpp"
#include "PlayerControlledBox/PlayerControlledBoxClient.hpp"
#include "PlayerControlledBox/PlayerControlledBoxServer.hpp"
#include <Network/NetworkManagerClientScript.hpp>
#include <Network/NetworkManagerServerScript.hpp>
#include "Weapons/WeaponHolders/GrenadeBunchScript.hpp"
#include "Weapons/WeaponGenerator.hpp"
#include <MusicManager.hpp>
#include "PlayerProgress/HealthBarHolder.hpp"

namespace null {

    // todo this is a dummy implementation
    // later reimplement this by loading stuff from file 
    // and using a resource manager
    void SceneLoader::loadSceneFromFile(const std::filesystem::path& path) {

        // a temporary solution to get a scene by keyword
        // while we don't store them in files
        auto keyword = path.string();

        const std::unordered_map<std::string, std::function<std::shared_ptr<Scene>(void)>> keywordToLevelGetter = {
                {"/demo",                   getDemoScene},
                {"/menu",                   getMenuScene},
                {"/menu/play",              getPlayScene},
                {"/menu/play/createRoom",   getCreateRoomScene},
                {"/menu/play/joinRoom",     getJoinRoomScene},
                {"/game",                   getGameScene},
                {"/network-demo-client",    getNetworkDemoClientScene},
                {"/network-demo-server",    getNetworkDemoServerScene}
        };

        std::shared_ptr<Scene> scene;
        std::function<std::shared_ptr<Scene>(void)> sceneFunction;

        try {
            sceneFunction = keywordToLevelGetter.at(keyword);
        } catch (const std::out_of_range& e) {
            throw null::UnknownSceneException();
        }
        scene = sceneFunction();
        MainLoop::provideScene(scene);
    }

    static std::shared_ptr<Scene> getGameScene() {
        return nullptr;
    }

    std::shared_ptr<Scene> SceneLoader::getNetworkDemoClientScene() {
        MainLoop::attachWindow = false;
        auto newScene = std::make_shared<Scene>();
        auto& box2dWorld = newScene->getBox2dWorld();

        auto clientManagerObject = std::make_shared<GameObject>(200200);
        clientManagerObject->addTag("network-manager");
        auto& clientScript =
                clientManagerObject->addScript<NetworkManagerClientScript>(*clientManagerObject);
        clientScript.ipToConnectTo = "127.0.0.1";
        clientScript.port = 5002;
        newScene->addRootGameObject(std::move(clientManagerObject));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> randX(0, 1280);
        std::uniform_real_distribution<> randY(0, 920);

        constexpr uint32_t totalBoxes = 15;
        for (uint32_t i = 0; i < totalBoxes; i++) {
            auto x = static_cast<float>(randX(rng));
            auto y = static_cast<float>(randY(rng));
            auto boxObject = std::make_shared<GameObject>(i);
            boxObject->setPosition(x, y);
            boxObject->addScript<PlayerControlledBoxClient>(*boxObject);
            newScene->addRootGameObject(std::move(boxObject));
        }

        return newScene;
    }

    std::shared_ptr<Scene> SceneLoader::getNetworkDemoServerScene() {
        MainLoop::attachWindow = true;
        auto newScene = std::make_shared<Scene>();
        auto& box2dWorld = newScene->getBox2dWorld();

        auto serverNetworkManager = std::make_shared<GameObject>(200200);
        serverNetworkManager->addTag("network-manager");
        auto& serverScript =
                serverNetworkManager->addScript<NetworkManagerServerScript>(*serverNetworkManager);
        newScene->addRootGameObject(std::move(serverNetworkManager));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> randX(0, 1280);
        std::uniform_real_distribution<> randY(0, 920);

        constexpr uint32_t totalBoxes = 15;
        for (uint32_t i = 0; i < totalBoxes; i++) {
            auto x = static_cast<float>(randX(rng));
            auto y = static_cast<float>(randY(rng));
            auto boxObject = std::make_shared<GameObject>(i);
            boxObject->setPosition(x, y);
            boxObject->addScript<PlayerControlledBoxServer>(*boxObject);
            newScene->addRootGameObject(std::move(boxObject));
        }

        return newScene;
    }

    std::shared_ptr<Scene> SceneLoader::getDemoScene() {

        // IMPORTANT!!!
        // If you want to bypass deserialization or get the scene serialized, comment next two lines
        // Otherwise leave them alone
//         auto sc = Serializer::getSceneFromFile("myscene.pbuf");
//         return sc;

        // todo this should be done in a scene file
        auto newScene = std::make_shared<Scene>();
        auto& box2dWorld = newScene->getBox2dWorld();

        auto musicManager = std::make_shared<GameObject>();
        auto& musicManagerScript = musicManager->addScript<MusicManager>(*musicManager);
        musicManagerScript.musicNameToLoad = "game-theme-synth.ogg";

        newScene->camera->addScript<ExampleCameraScript>(*newScene->camera);
        // this texture is not released on purpose, because it MUST exist for as long
        // as the sprite lives. todo manage it with resource manager
        sf::Texture* nullTexture = ResourceManager::loadTexture("background.png");
        auto parentGameObject = std::make_shared<GameObject>();
        auto weaponGenerator = std::make_shared<GameObject>();

        weaponGenerator->addScript<WeaponGenerator>(*weaponGenerator);
        parentGameObject->addChild(std::move(weaponGenerator));

        auto nullGameLogo = std::make_shared<GameObject>();
        nullGameLogo->getSprite().setTexture(*nullTexture);
        nullGameLogo->getSprite().setScale({8.0f, 8.0f});
        nullGameLogo->renderLayer = serial::BACKGROUND;
        nullGameLogo->visible = true;

        auto boxTexture = ResourceManager::loadTexture("box.png");

        auto boxObject = std::make_shared<GameObject>();
        boxObject->getSprite().setTexture(*boxTexture);
        boxObject->getSprite().setScale(0.125f, 0.125f);
        boxObject->setPosition(200, 0);
        boxObject->renderLayer = serial::FOREGROUND;
        boxObject->visible = true;

        auto boxObject2 = std::make_shared<GameObject>();
        boxObject2->getSprite().setTexture(*boxTexture);
        boxObject2->getSprite().setScale(0.125f, 0.125f);
        boxObject2->setPosition(750.0f, 200.0f);
        boxObject2->getSprite().setColor(sf::Color(255U, 0U, 0U));
        boxObject2->renderLayer = serial::BACKGROUND1;
        boxObject2->visible = true;
        auto createGround = [&box2dWorld, &newScene](float x, float y) {
            auto groundObject = std::make_shared<GameObject>();
            auto& groundSprite = groundObject->getSprite();
            groundSprite.setTexture(*ResourceManager::loadTexture("platform.png"));
            groundSprite.setScale(3.0f, 3.0f);
            groundSprite.setPosition(x, y);
            groundObject->renderLayer = serial::FOREGROUND;
            groundObject->visible = true;
            groundObject->addTag("platform");
            groundObject->makeStatic(box2dWorld);
            groundObject->addScript<ReloadSceneScript>(*groundObject);
            newScene->addRootGameObject(std::move(groundObject));
        };
        createGround(0, 400);
        createGround(192, 466);
        createGround(384, 532);
        createGround(576, 532);
        createGround(1152, 400);
        createGround(880, 100);
        for (int i = 0; i < 10; i++) {
            createGround(i * 300, i * 200);
        }
        boxObject->makeDynamic(box2dWorld);

        auto cursorObject = std::make_shared<GameObject>(std::set<std::string>({"cursor"}));

        auto spriteSheet = SpriteSheet("cursorAnim.png", sf::Vector2i(16, 16), {{"cursorAnim", 0, 0, 5}});
        cursorObject->addScript<CursorAnimation>(*cursorObject, spriteSheet);
        cursorObject->addTag("cursor");
        cursorObject->renderLayer = serial::FOREGROUND3;
        cursorObject->visible = true;

        auto player = PlayerAnimation::initPlayer("playerAnim_v2.png", box2dWorld);
        auto grenadeBunch = std::make_shared<GameObject>();
        grenadeBunch->addScript<GrenadeBunchScript>(*grenadeBunch);

        player->getScript<PlayerAnimation>()->controlled = true;
        auto enemy1 = PlayerAnimation::initPlayer("playerAnim_v3.png", box2dWorld);
        auto enemy2 = PlayerAnimation::initPlayer("playerAnim_v3.png", box2dWorld);
        auto enemy3 = PlayerAnimation::initPlayer("playerAnim_v3.png", box2dWorld);
        auto enemy4 = PlayerAnimation::initPlayer("playerAnim_v3.png", box2dWorld);
//        enemy1->setPosition(300, 0);
//        enemy2->setPosition(200, 200);
//        enemy3->setPosition(400, 000);
        enemy4->setPosition(400, 200);
//        enemy1->getScript<PlayerAnimation>()->name = "Meow";
//        enemy2->getScript<PlayerAnimation>()->name = "Gav";
//        enemy4->getScript<PlayerAnimation>()->name = "Meowss";
//        enemy3->getScript<PlayerAnimation>()->name = "Gavaa";

        auto gun = std::make_shared<GameObject>();
        gun->addScript<StraightWeaponScript>(*gun, 0.01);

        auto weaponStorage = std::make_shared<GameObject>();
        std::vector<std::shared_ptr<GameObject>> guns{gun, grenadeBunch};
        weaponStorage->addScript<WeaponStorage>(*weaponStorage, guns);

        player->addChild(std::move(weaponStorage));
        newScene->camera->getScript<ExampleCameraScript>()->setTrackedGameObject(*player);
        newScene->camera->getScript<ExampleCameraScript>()->setMap(*nullGameLogo);

        auto healthBarHolder = std::make_shared<GameObject>();
        healthBarHolder->addScript<HealthBarHolder>(*healthBarHolder);
        parentGameObject->addChild(std::move(healthBarHolder));

        MapManager mapManager(box2dWorld);
        parentGameObject->addChild(std::move(mapManager.makeBorder(nullGameLogo->getSprite())));
        nullGameLogo->addChild(std::move(boxObject));
        parentGameObject->addChild(std::move(nullGameLogo));
        parentGameObject->addChild(std::move(player));
        parentGameObject->addChild(std::move(cursorObject));
//        parentGameObject->addChild(std::move(enemy1));
//        parentGameObject->addChild(std::move(enemy2));
//        parentGameObject->addChild(std::move(enemy3));
        parentGameObject->addChild(std::move(enemy4));
        newScene->addRootGameObject(std::move(parentGameObject));
        newScene->addRootGameObject(std::move(musicManager));

        Serializer::serializeSceneToFile(newScene.get(), "myscene.pbuf");
        return newScene;
    }

    std::shared_ptr<Scene> SceneLoader::getPlayScene() {
        return nullptr;
    }

    std::shared_ptr<Scene> SceneLoader::getCreateRoomScene() {
        return nullptr;
    }

    std::shared_ptr<Scene> SceneLoader::getJoinRoomScene() {
        return nullptr;
    }

    std::shared_ptr<Scene> SceneLoader::getGameScene() {
        return nullptr;
    }

    std::shared_ptr<Scene> SceneLoader::getMenuScene() {

        auto newScene = std::make_shared<Scene>();
        auto& box2dWorld = newScene->getBox2dWorld();
        sf::Texture* nullTexture = ResourceManager::loadTexture("menu/menu_background.png");

        auto musicManager = std::make_shared<GameObject>();
        auto& musicManagerScript = musicManager->addScript<MusicManager>(*musicManager);
        musicManagerScript.musicNameToLoad = "game-theme-synth.ogg";

        auto background = std::make_shared<GameObject>();
        background->getSprite().setTexture(*nullTexture);
        background->getSprite().setPosition({0, 0});
        background->renderLayer = serial::BACKGROUND;
        background->visible = true;

        auto cursorObject = std::make_shared<GameObject>(std::set<std::string>({"cursor"}));

        auto spriteSheet = SpriteSheet("cursorAnim.png", sf::Vector2i(16, 16), {{"cursorAnim", 0, 0, 5}});
        cursorObject->addScript<CursorAnimation>(*cursorObject, spriteSheet);
        cursorObject->renderLayer = serial::FOREGROUND3;
        cursorObject->addTag("cursor");
        cursorObject->visible = true;
        cursorObject->getSprite().setScale(0.1f, 0.1f);
        cursorObject->makeDynamic(box2dWorld);
        cursorObject->getSprite().setScale(4.f, 4.f);
        cursorObject->getRigidBody()->GetFixtureList()->SetSensor(true);

        sf::Texture* pressedTexture = ResourceManager::loadTexture("menu/buttons/null_text.png");

        auto playButton = std::make_shared<GameObject>();
        auto playButtonTexture = ResourceManager::loadTexture("menu/buttons/play.png");
        playButton->setPosition(450, 380);
        playButton->addScript<ButtonScript>(*playButton, *playButtonTexture, *pressedTexture, []() -> void {
            SceneLoader::changeScene("/demo");
        });
        playButton->renderLayer = serial::FOREGROUND;
        playButton->visible = true;

        auto exitButton = std::make_shared<GameObject>();
        auto exitButtonTexture = ResourceManager::loadTexture("menu/buttons/exit.png");
        exitButton->setPosition(450, 600);
        exitButton->addScript<ButtonScript>(*exitButton, *exitButtonTexture, *pressedTexture, []() -> void {
            std::exit(0);
        });
        exitButton->renderLayer = serial::FOREGROUND;
        exitButton->visible = true;


        auto optionsButton = std::make_shared<GameObject>();
        auto optionsButtonTexture = ResourceManager::loadTexture("menu/buttons/options.png");
        optionsButton->setPosition(320, 490);
        optionsButton->addScript<ButtonScript>(*optionsButton, *optionsButtonTexture, *pressedTexture,
                                               []() -> void {});
        optionsButton->renderLayer = serial::FOREGROUND;
        optionsButton->visible = true;


        newScene->addRootGameObject(std::move(musicManager));
        newScene->addRootGameObject(std::move(background));
        newScene->addRootGameObject(std::move(cursorObject));
        newScene->addRootGameObject(std::move(playButton));
        newScene->addRootGameObject(std::move(optionsButton));
        newScene->addRootGameObject(std::move(exitButton));

        return newScene;
    }

    /**
     * Reload current game tree and restart simulation
     * @param path path to level to load
     */
    void SceneLoader::changeScene(const std::filesystem::path& path) {
        try {
            loadSceneFromFile(path);
        } catch (const null::UnknownSceneException& ignored) {
            std::cerr << "SceneLoader::changeScene tried to load a non-existent scene" << std::endl;
            exit(-1);
        }
        throw SceneChangedException(); // a mean of flow control
    }

}
