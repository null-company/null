#include <Entity.hpp>
#include <Utility.hpp>

namespace null {
    Entity::Entity(): Entity(null::Utility::generateGuid()) { }

    Entity::Entity(uint64_t guid): guid(guid) { }
}
