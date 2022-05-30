#pragma once

#include <memory>

#include <SFML/System.hpp>
#include <SFML/Audio/Music.hpp>

#include <GameObject.hpp>

namespace null {

    class MusicManager : public Script {
    public:
        std::string musicNameToLoad{};

        void start() override;

        void update() override;

        explicit MusicManager(GameObject& gameObject);
        ~MusicManager() override;

        void serialize(google::protobuf::Message& message) const;

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message& message);

    private:
        std::shared_ptr<sf::Music> music{};
    };

}

