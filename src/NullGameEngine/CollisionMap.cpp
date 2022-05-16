#include <CollisionMap.hpp>
#include <utility>
#include "scripts.pb.h"
#include <box2d/b2_polygon_shape.h>

namespace null {
    void CollisionMap::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::CollisionMap&>(message);
        for (const auto& [name, val] : collisionMapInternal) {
            // Add an entry to map and set name
            auto t = msg.add_collision_map_entry();
            t->set_name(name);
            for (const auto& i : val) {
                // add list of fixtures for a single frame
                auto tt = t->add_fixture_def_list();
                for (const auto& j : i) {
                    // finally add fixtures for each hitbox
                    auto ttt = tt->add_fixture_def();
                    ttt->set_density(j.density);
                    ttt->set_friction(j.friction);
                    ttt->set_is_sensor(j.isSensor);
                    ttt->set_restitution(j.restitution);
                    ttt->set_restitution_threshold(j.restitutionThreshold);
                    ttt->mutable_filter()->set_category_bits(j.filter.categoryBits);
                    ttt->mutable_filter()->set_group_index(j.filter.groupIndex);
                    ttt->mutable_filter()->set_mask_bits(j.filter.maskBits);
                    auto pshape = dynamic_cast<const b2PolygonShape*>(j.shape);
                    for (auto k = 0; k < pshape->m_count; k++) {
                        auto vertex = pshape->m_vertices[k];
                        auto tempthing = ttt->mutable_shape()->mutable_polygon()->add_vertices();
                        tempthing->set_x(vertex.x);
                        tempthing->set_y(vertex.y);
                    }
                }
            }

        }
    }

    std::unique_ptr<null::CollisionMap> null::CollisionMap::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::CollisionMap&>(message);
        auto cmi = CollisionMapInternal{};
        for (const auto& collisionMapEntry : msg.collision_map_entry()) {
            std::vector<FixtureDefVec> gigaList{};
            for (const auto& fixtureDefList : collisionMapEntry.fixture_def_list()) {
                FixtureDefVec fdv{};
                for (const auto& s_fixtureDef : fixtureDefList.fixture_def()) {
                    b2FixtureDef fd{};
                    fd.density = s_fixtureDef.density();
                    fd.restitution = s_fixtureDef.restitution();
                    fd.restitutionThreshold = s_fixtureDef.restitution_threshold();
                    fd.isSensor = s_fixtureDef.is_sensor();
                    fd.friction = s_fixtureDef.friction();
                    fd.filter.maskBits = s_fixtureDef.filter().mask_bits();
                    fd.filter.groupIndex = s_fixtureDef.filter().group_index();
                    fd.filter.categoryBits = s_fixtureDef.filter().category_bits();
                    // TODO this is stupid
                    // box2d clones the shape ON THE MOMENT OF FIXTURE CREATION
                    // so WE have to make sure it's not deleted until then
                    // so the only way to do that is to create one on the heap
                    // and cry because of obvious memory leakage
                    // please suggest solutions if u can
                    auto not_a_memory_leak = new b2PolygonShape();
                    std::vector<b2Vec2> pshape;
                    for (const auto& vertex : s_fixtureDef.shape().polygon().vertices()) {
                        pshape.emplace_back(vertex.x(), vertex.y());
                    }
                    not_a_memory_leak->Set(pshape.data(), (int) pshape.size());
                    fd.shape = not_a_memory_leak;
                    fdv.push_back(fd);
                }
                gigaList.push_back(fdv);
            }
            cmi.try_emplace(collisionMapEntry.name(), gigaList);
        }
        return std::make_unique<CollisionMap>(cmi);
    }

    null::CollisionMap::CollisionMap(null::CollisionMapInternal  collisionMapInternal1)
            : collisionMapInternal(std::move(collisionMapInternal1)) { }

}
