#include "CameraScript.hpp"
#include "Serializer.hpp"
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

    void CameraScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        auto s_script = msg.mutable_camera_script();
        auto s_view = s_script->mutable_view();
        s_view->mutable_center()->set_x(view.getCenter().x);
        s_view->mutable_center()->set_y(view.getCenter().y);
        s_view->mutable_size()->set_x(view.getSize().x);
        s_view->mutable_size()->set_y(view.getSize().y);
    }

    std::unique_ptr<Component> CameraScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto p_script = std::make_unique<CameraScript>(
                *Serializer::currentDeserializationGameObject
                );
        auto& s_script = msg.camera_script();
        p_script->view.setSize(s_script.view().size().x(), s_script.view().size().y());
        p_script->view.setCenter(s_script.view().center().x(), s_script.view().center().y());
        return p_script;
    }
}
