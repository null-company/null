#pragma once

#include <memory>

#include <SFML/System.hpp>
#include <SFML/Audio/Music.hpp>

#include <GameObject.hpp>

namespace null {

    class MusicManager : public Script {
    public:
        std::string musicNameToLoad{};
    public:
        void start() override;

        void update() override;

        explicit MusicManager(GameObject& gameObject);

//        void serialize(google::protobuf::Message &) const override;
//        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    private:
        std::unique_ptr<sf::Music> music{};
    };

}

