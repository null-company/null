#include "ExampleCameraScript.hpp"

namespace null{

    void ExampleCameraScript::start() {
        camera.view = sf::View(sf::FloatRect(0, 0, 1280, 720));
    }

    void ExampleCameraScript::update() {
        camera.view.setCenter((sf::Vector2f)sf::Mouse::getPosition());
    }

    ExampleCameraScript::ExampleCameraScript(Camera& camera) : CameraScript(camera) { }
}