//
// Created by artemonchelus on 30.05.22.
//

#include <TextHandler.hpp>
#include <GameObject.hpp>
#include <SFML/Graphics.hpp>
#include <ResourceManager.hpp>
#include <Scene.hpp>
#include "ButtonScript.hpp"

namespace null {
    void TextHandler::start() {
        Component::start();
        if (!active) {
            return;
        }
        gameObject.getText().emplace();
        gameObject.getText()->setFont(*ResourceManager::loadFont("NULL.ttf"));

        gameObject.getText()->setFillColor(sf::Color(000, 204, 204));
        gameObject.getText()->setCharacterSize(40);
        gameObject.getText()->setPosition(gameObject.getPosition() + sf::Vector2f{400, 5});

        auto textBackGameObject = std::make_shared<GameObject>();
        textBackGameObject->addTag("text back");
        textBackGameObject->getText().emplace();
        textBackGameObject->getText()->setFont(*ResourceManager::loadFont("NULL.ttf"));
        textBackGameObject->getText()->setFillColor(sf::Color(255, 000, 175));
        textBackGameObject->getText()->setCharacterSize(gameObject.getText()->getCharacterSize());
        textBackGameObject->getText()->setPosition(gameObject.getText()->getPosition() + sf::Vector2f{5, 5});
        textBackGameObject->renderLayer = serial::BACKGROUND;
        textBackGameObject->visible = true;

        gameObject.addChild(std::move(textBackGameObject));
    }

    void TextHandler::update() {
        if (!active) {
            return;
        }
        if (!wasStarted) {
            wasStarted = true;
            gameObject.getScript<ButtonScript>()->active = false;
            TextHandler::start();
        }
        char enteredChar = gameObject.getSceneForce().windowMetaInfo.enteredChar;
        auto backGo = gameObject.findFirstChildrenByTag("text back");
        if (enteredChar != '\0') {
            if (std::isalpha(enteredChar) || std::isdigit(enteredChar)) {
                if (gameObject.getText()->getString().getSize() != 6) {
                    enteredChar = !std::isdigit(enteredChar) ? std::toupper(enteredChar) : enteredChar;
                    gameObject.getText()->setString(gameObject.getText()->getString() + enteredChar);
                    backGo->getText()->setString(backGo->getText()->getString() + enteredChar);
                };
            } else if (enteredChar == 8 || enteredChar == 127) { // 8 is backslash, 127 is del character
                auto string = gameObject.getText()->getString();
                if (!string.isEmpty()) {
                    gameObject.getText()->setString(string.substring(0, string.getSize() - 1));
                    backGo->getText()->setString(gameObject.getText()->getString());
                }
            }
        }
        Component::update();
    }

    TextHandler::TextHandler(GameObject& object) : Component(object) {}

    void TextHandler::serialize(google::protobuf::Message& message) const {
        Component::serialize(message);
    }
} // null