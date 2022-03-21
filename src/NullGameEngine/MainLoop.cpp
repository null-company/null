#include <memory>
#include <thread>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include <Utility.hpp>
#include <NetworkPlayerScript.hpp>
#include <exceptions/NetworkException.h>

namespace null {

    constexpr unsigned int MAX_FRAMERATE = 60;

    std::shared_ptr<Scene> MainLoop::scene = nullptr;
    ClientNetworkManager MainLoop::clientNetworkManager = ClientNetworkManager("127.0.0.1", 5000);

    // todo this is a dummy implementation, copied from the earlier draft
    int MainLoop::run() {
        sf::RenderWindow sfmlWin(sf::VideoMode(1280, 720), "{[Null]}");
        sfmlWin.setFramerateLimit(MAX_FRAMERATE);

        auto renderingThread = [](sf::RenderWindow& window) {
            window.setActive(true);
            while (window.isOpen()) {
                window.clear(sf::Color::Black);
                Renderer::render(window, *MainLoop::scene);
                window.display();
            }
        };

        sf::Thread rendererThread(renderingThread, std::ref(sfmlWin));

        std::unordered_set<uint> gg;
        auto& q = clientNetworkManager.subscribe(-1337, net::GameMessage::kPlayerInfo);

        if (std::getenv("ROOM_CREATOR")) {
            clientNetworkManager.getClient().createRoom();
            LOGD << clientNetworkManager.getClient().getRoom();
        } else {
            clientNetworkManager.getClient().connectRoom("AAAAAA");
        }

        sceneStart:
        scene->start();

        try {
            while (sfmlWin.isOpen()) {
                try {
                    while (true) {
                        auto message = receiveNetMessage(
                                clientNetworkManager.getClient().getGameServerSocket()).game_message();
                        clientNetworkManager.multiplexMessage(message);
                    }
                } catch (ReceiveException& e) {
                    //LOGD << "Read all the messages";
                }

                while (!q.empty()) {
                    auto t = q.front();
                    if (gg.count(t.game_id()) == 0) {
                        gg.emplace(t.game_id());
                        auto player = std::make_unique<GameObject>();
                        player->getSprite().setTextureRect({0, 0, 30, 54});
                        player->getSprite().setScale(3.0f, 3.0f);
                        //player->setPosition(100, 0);
                        player->visible = true;
                        player->renderLayer = FOREGROUND1;
                        player->makeDynamic(scene->getBox2dWorld());
                        player->getRigidBody()->SetFixedRotation(true);
                        auto playerSpriteSheet = SpriteSheet("playerAnim_v3.png", {30, 54}, {{"idle",      0, 0, 7},
                                                                                             {"walkRight", 1, 0, 3},
                                                                                             {"walkLeft",  2, 0, 3}});

                        auto shape1 = new b2PolygonShape();
                        auto sizeVector = Utility::pixelToMetersVector(sf::Vector2i{39, 162});
                        shape1->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(),
                                         0.0f);
                        b2FixtureDef fixtureDef1;
                        fixtureDef1.shape = shape1;
                        fixtureDef1.density = 1;

                        auto shape2 = new b2PolygonShape();
                        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{87, 162});
                        shape2->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(),
                                         0.0f);
                        b2FixtureDef fixtureDef2;
                        fixtureDef2.shape = shape2;
                        fixtureDef2.density = 1;

                        auto shape3 = new b2PolygonShape();
                        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{78, 162});
                        shape3->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(),
                                         0.0f);
                        b2FixtureDef fixtureDef3;
                        fixtureDef3.shape = shape3;
                        fixtureDef3.density = 1;

                        auto& tq = clientNetworkManager.subscribe(t.game_id(), net::GameMessage::kPlayerInfo);
                        player->addScript<NetworkPlayerScript>(*player, playerSpriteSheet,
                                                               std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>{
                                                                       {"idle",      {{fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}}},
                                                                       {"walkRight", {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}},
                                                                       {"walkLeft",  {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}}
                                                               }, tq, t.game_id());
                        tq.push(t);
                        scene->addRootGameObject(std::move(player));
                    }
                    q.pop();
                }


                scene->update();

                // todo dispatching user events such as keyboard inputs will probably take place here
                sf::Event e;
                while (sfmlWin.pollEvent(e)) {
                    switch (e.type) {
                        case sf::Event::EventType::Closed:
                            sfmlWin.close();
                            break;
                        default:
                            break;
                    }
                }
                sfmlWin.clear(sf::Color::Black);
                Renderer::render(sfmlWin, *MainLoop::scene);
                sfmlWin.display();
            }
        }
        catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }

        return 0;
    }

    void createNewNetworkPlayerObject() {

        auto player = std::make_unique<GameObject>();
        player->getSprite().setTextureRect({0, 0, 30, 54});
        player->getSprite().setScale(3.0f, 3.0f);
        //player->setPosition(100, 0);
        player->visible = true;
        player->renderLayer = FOREGROUND1;
        //player->makeDynamic(box2dWorld);
        player->getRigidBody()->SetFixedRotation(true);
        auto playerSpriteSheet = SpriteSheet("playerAnim_v3.png", {30, 54}, {{"idle",      0, 0, 7},
                                                                             {"walkRight", 1, 0, 3},
                                                                             {"walkLeft",  2, 0, 3}});

        auto shape1 = new b2PolygonShape();
        auto sizeVector = Utility::pixelToMetersVector(sf::Vector2i{39, 162});
        shape1->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(), 0.0f);
        b2FixtureDef fixtureDef1;
        fixtureDef1.shape = shape1;
        fixtureDef1.density = 1;

        auto shape2 = new b2PolygonShape();
        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{87, 162});
        shape2->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(), 0.0f);
        b2FixtureDef fixtureDef2;
        fixtureDef2.shape = shape2;
        fixtureDef2.density = 1;

        auto shape3 = new b2PolygonShape();
        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{78, 162});
        shape3->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(), 0.0f);
        b2FixtureDef fixtureDef3;
        fixtureDef3.shape = shape3;
        fixtureDef3.density = 1;

        std::queue<int> q;
//        player->addScript<NetworkPlayerScript>(*player, playerSpriteSheet,
//                                           std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>{
//                                                   {"idle", {{fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}}},
//                                                   {"walkRight", {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}},
//                                                   {"walkLeft", {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}}
//                                           }, q, 4);
        //TODO add player to scene
    }

}

