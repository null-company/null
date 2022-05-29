#pragma once

#include <queue>
#include <memory>

#include <network.h>

#include <Script.hpp>

namespace null {

    /**
     * A place to manage any server network deeds.
     * Is supposed to be launched at start as the first script
     */
    struct NetworkManagerServerScript: public Script {

        void start() override;
        void update() override;
        explicit NetworkManagerServerScript(GameObject& go);

        std::queue<net::GameMessage::ClientCommand>& subscribe(uint64_t guid);

        void serialize(google::protobuf::Message & message) const override;

        static std::unique_ptr<Script> deserialize(const google::protobuf::Message& message);

    private:
        GameServer* gameServer{};
    };

}
