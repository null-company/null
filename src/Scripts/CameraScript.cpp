#include "CameraScript.hpp"
#include <Camera.hpp>
#include <Scene.hpp>

namespace null {
    CameraScript::CameraScript(GameObject& camera) : Component(camera),
                                                     camera(camera) {}

    void CameraScript::start() {
        this->windowMetaInfo = &(camera.getScene().lock()->getWindowMetaInfo());
        this->view = sf::View(sf::FloatRect(0,
                                            0,
                                            windowMetaInfo->windowsSize.x,
                                            windowMetaInfo->windowsSize.y));
    }

    void CameraScript::update() {
        Component::update();
    }
}
