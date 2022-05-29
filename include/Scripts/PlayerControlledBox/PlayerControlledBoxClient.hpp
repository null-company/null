#pragma once

#include <queue>
#include <network.h>

#include <Script.hpp>
#include <Network/NetworkManagerClientScript.hpp>
#include <Network/PrimitiveStateConverter.hpp>
#include <Network/Utility/LastAcceptedMessageProcessor.hpp>

namespace null {

    class PlayerControlledBoxClient : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxClient(GameObject& go);

        void serialize(google::protobuf::Message & message) const override;

        static std::unique_ptr<Script> deserialize(const google::protobuf::Message& message);

    private:
        float x{}, y{};
        StateConverter networkStateManager{x, y}; // network state manager will be restoring these values
        LastAcceptedMessageProcessor<net::GameMessage::SubscriberState> messageQueue{};
        NetworkManagerClientScript* networkManagerScript{};
    };

}
