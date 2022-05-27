#pragma once 

#include <Script.hpp>
#include <GameObject.hpp>
#include <Scripts.hpp>
#include <ClockedScript.hpp>

namespace null {

    class ExampleClockedScript : public ClockedScript {
        public:

        void clockedStart() override;
        void clockedUpdate() override;

        explicit ExampleClockedScript(GameObject& gameObject);

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };
}

