#include "Network/PrimitiveStateConverter.hpp"

namespace null {

    namespace {
        enum VariantType : uint8_t {
            UINT32 = 0,
            UINT64 = 1,
            STRING = 2,
            FLOAT = 3,
            DOUBLE = 4,
            BOOL = 5
        };

    }

    void PrimitiveStateConverter::dumpToMessage(net::GameMessage::PrimitiveState& stateMessage) const {
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

    net::GameMessage::PrimitiveState PrimitiveStateConverter::makeStateMessage() const {
        net::GameMessage::PrimitiveState message;
        dumpToMessage(message);
        return message;
    }

    void PrimitiveStateConverter::restoreFromMessage(const net::GameMessage::PrimitiveState& stateMessage) const {
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
}
