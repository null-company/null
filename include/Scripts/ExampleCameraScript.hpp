#pragma once

#include <Scripts.hpp>
#include <Scene.hpp>
#include <Network/PlayerDispatchers/PlayerDispatcherClient.hpp>

namespace null {
    class ExampleCameraScript : public CameraScript {
    public:
        float scale = 1;
        GameObject* map = nullptr;
        GameObject* trackedObject = &gameObject;
    public:

        void start() override;

        void update() override;

        explicit ExampleCameraScript(GameObject& camera);

        void setTrackedGameObject(GameObject& gameObject);

        void setMap(GameObject& object);

        void serialize(google::protobuf::Message&) const override;

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

        float getScale() const;

        void setScale(float scale);
    private:
        sf::FloatRect computeNewViewRect();
    };

    class CurrentPlayerCameraScript : public ExampleCameraScript {
    public:
        explicit CurrentPlayerCameraScript(GameObject& camera) : ExampleCameraScript(camera) { };

        void start() override {
            const auto& clientDispatcher =
                    gameObject.getSceneForce().findFirstByTag("client-player-dispatcher").lock();
            const auto& clientDispatcherScript =
                    clientDispatcher->getScript<PlayerDispatcherClient>();
            clientDispatcherScript->registerPlayerFindingObserver([this](const std::string& playerTag) {
                auto& playerObject =
                        *gameObject.getSceneForce().findFirstByTag(playerTag).lock();
                setTrackedGameObject(playerObject);
            });
            ExampleCameraScript::start();
        }

    };
}
