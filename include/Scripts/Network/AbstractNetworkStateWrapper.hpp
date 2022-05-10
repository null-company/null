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
    class AbstractNetworkStateWrapper {
    private:
        using allowedVariant = std::variant<uint32_t *, uint64_t *, std::string *, float *, double *, bool *>;
        enum VariantType : uint8_t {
            UINT32 = 0,
            UINT64 = 1,
            STRING = 2,
            FLOAT = 3,
            DOUBLE = 4,
            BOOL = 5
        };

    private:
        std::vector<allowedVariant> propsToManage{};
        AbstractNetworkStateWrapper() = default;

    public:

        template<typename T>
        explicit AbstractNetworkStateWrapper(T& t) {
            propsToManage.emplace_back(&t);
        }

        template<typename T, typename... Args>
        explicit AbstractNetworkStateWrapper(T& t, Args& ... args) : AbstractNetworkStateWrapper(args...) {
            propsToManage.emplace_back(&t);
        }

        /**
         * Dump watched props values to given message. Expects an empty PrimitiveState message
         *
         * Note: things such as id should be set outside and can't be managed by this class
         * @param stateMessage message object in which to write current state of watched properties
         */
        void serializeStateToMessage(net::GameMessage::PrimitiveState& stateMessage) const {
            for (const auto& prop: propsToManage) {
                auto type = static_cast<VariantType>(prop.index());
                switch (type) {
                    case UINT32: {
                        auto ui32p = std::get<UINT32>(prop);
                        stateMessage.add_uint32s(*ui32p);
                        break;
                    }
                    case UINT64: {
                        auto ui64p = std::get<UINT64>(prop);
                        stateMessage.add_uint64s(*ui64p);
                        break;
                    }
                    case STRING: {
                        auto sp = std::get<STRING>(prop);
                        stateMessage.add_strings(*sp);
                        break;
                    }
                    case FLOAT: {
                        auto fp = std::get<FLOAT>(prop);
                        stateMessage.add_floats(*fp);
                        break;
                    }
                    case DOUBLE: {
                        auto dp = std::get<DOUBLE>(prop);
                        stateMessage.add_doubles(*dp);
                        break;
                    }
                    case BOOL: {
                        auto bp = std::get<BOOL>(prop);
                        stateMessage.add_bools(*bp);
                        break;
                    }
                }
            }
        }

        [[nodiscard]]
        net::GameMessage::PrimitiveState makeStateMessage() const {
            net::GameMessage::PrimitiveState message;
            serializeStateToMessage(message);
            return message;
        }

        template <typename... Args>
        static net::GameMessage::PrimitiveState makeStateMessageFrom(Args ... props) {
            return AbstractNetworkStateWrapper(props...).makeStateMessage();
        }

        template <typename... Args>
        static void restoreStateFromMessage(const net::GameMessage::PrimitiveState& stateMessage, Args& ... props) {
            AbstractNetworkStateWrapper(props...).restoreStateFromMessage(stateMessage);
        }

        /**
         * Given a message, restore watched properties to the serialized state
         * @param stateMessage message to restore from
         */
        void restoreStateFromMessage(const net::GameMessage::PrimitiveState& stateMessage) const {
            uint16_t uint32Index = 0;
            uint16_t uint64Index = 0;
            uint16_t stringIndex = 0;
            uint16_t floatIndex = 0;
            uint16_t doubleIndex = 0;
            uint16_t boolIndex = 0;

            for (const auto& prop: propsToManage) {
                auto type = static_cast<VariantType>(prop.index());
                switch (type) {
                    case UINT32: {
                        auto ui32p = std::get<UINT32>(prop);
                        *ui32p = stateMessage.uint32s(uint32Index++);
                        break;
                    }
                    case UINT64: {
                        auto ui64p = std::get<UINT64>(prop);
                        *ui64p = stateMessage.uint64s(uint64Index++);
                        break;
                    }
                    case STRING: {
                        auto sp = std::get<STRING>(prop);
                        *sp = stateMessage.strings(stringIndex++);
                        break;
                    }
                    case FLOAT: {
                        auto fp = std::get<FLOAT>(prop);
                        *fp = stateMessage.floats(floatIndex++);
                        break;
                    }
                    case DOUBLE: {
                        auto dp = std::get<DOUBLE>(prop);
                        *dp = stateMessage.doubles(doubleIndex++);
                        break;
                    }
                    case BOOL: {
                        auto bp = std::get<BOOL>(prop);
                        *bp = stateMessage.bools(boolIndex++);
                        break;
                    }
                }
            }
        }
    };

//    using NetworkStateManager = AbstractNetworkStateWrapper<net::GameMessage::SubscriberState>;
//    using NetworkCommandManager = AbstractNetworkStateWrapper<net::GameMessage::ClientCommand>;
    using NetworkStateManager = AbstractNetworkStateWrapper;
    using NetworkCommandManager = AbstractNetworkStateWrapper;
}
