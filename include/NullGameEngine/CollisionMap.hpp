#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <box2d/b2_fixture.h>
#include <google/protobuf/message.h>

namespace null {
    using FixtureDefVec = std::vector<b2FixtureDef>;
    using CollisionMapInternal = std::unordered_map<std::string, std::vector<FixtureDefVec>>;

    // This is the class that goes with a RigidBodyAnimation
    // Holds the fixtureDefs to create fixtures for each frame of animation
    class CollisionMap {
    public:
        CollisionMapInternal collisionMapInternal;

        explicit CollisionMap(CollisionMapInternal );
        void serialize(google::protobuf::Message&) const;
        static std::unique_ptr<CollisionMap> deserialize(const google::protobuf::Message&);
    };
}
