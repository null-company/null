#pragma once

#include <vector>
#include <variant>
#include <string>

#include "serialized/serverConfig.pb.h"

/*
 * Reference for the message schema
  message PrimitiveState {
    repeated google.protobuf.Any anys = 1; // TODO(Roman) that is not really handled. Should it be?
    repeated uint32 uint32s = 2;
    repeated uint64 uint64s = 3;
    repeated string strings = 4;
    repeated float floats = 5;
    repeated double doubles = 6;
    repeated bool bools = 7;
  }
 */
namespace null {
    class PrimitiveStateConverter {
    private:
        using allowedVariant = std::variant<uint32_t *, uint64_t *, std::string *, float *, double *, bool *>;
    private:
        std::vector<allowedVariant> propsToManage{};
        PrimitiveStateConverter() = default;

    public:

        template<typename T>
        explicit PrimitiveStateConverter(T& t) {
            propsToManage.emplace_back(&t);
        }

        template<typename T, typename... Args>
        explicit PrimitiveStateConverter(T& t, Args& ... args) : PrimitiveStateConverter(args...) {
            propsToManage.emplace_back(&t);
        }

        /**
         * Dump watched props values to given message. Expects an empty PrimitiveState message
         *
         * Note: things such as id should be set outside and can't be managed by this class
         * @param stateMessage message object in which to write current state of watched properties
         */
        void dumpToMessage(net::GameMessage::PrimitiveState& stateMessage) const;

        [[nodiscard]]
        net::GameMessage::PrimitiveState makeStateMessage() const;

        template <typename... Args>
        static net::GameMessage::PrimitiveState makeMessageFrom(Args ... props) {
            return PrimitiveStateConverter(props...).makeStateMessage();
        }

        template <typename... Args>
        static void restoreFromMessage(const net::GameMessage::PrimitiveState& stateMessage, Args& ... props) {
            PrimitiveStateConverter(props...).restoreFromMessage(stateMessage);
        }

        /**
         * Given a message, restore watched properties to the serialized state
         * @param stateMessage message to restore from
         */
        void restoreFromMessage(const net::GameMessage::PrimitiveState& stateMessage) const;
    };

    using StateConverter = PrimitiveStateConverter;
    using CommandConverter = PrimitiveStateConverter;
}
