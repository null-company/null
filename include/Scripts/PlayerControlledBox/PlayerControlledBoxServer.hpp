#pragma once

#include <queue>
#include <SFML/System.hpp>

#include <Script.hpp>
#include "serialized/serverConfig.pb.h"
#include <Network/PrimitiveStateConverter.hpp>
#include <Network/Utility/LastAcceptedMessageProcessor.hpp>

namespace null {

    class PlayerControlledBoxServer : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxServer(GameObject& go) : Script(go) {}

    private:
        LastAcceptedMessageProcessor<net::GameMessage::ClientCommand> messageQueue{};
        sf::Clock lastStateSnapshotTimer{};
    };

}
