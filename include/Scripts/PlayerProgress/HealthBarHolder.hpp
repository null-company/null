#pragma once

#include <deque>
#include "Script.hpp"
#include "Schedulers/Timer.hpp"
#include "chrono"
#include "deque"
#include "SFML/Graphics.hpp"

namespace null {
    // Track game objects with tag player
    // U must provide a name to player to make a difference between them
    class HealthBarHolder : public Script {
    private:

        std::map<std::string, std::shared_ptr<GameObject>> nameToBar;
        std::map<std::string, std::weak_ptr<GameObject>> nameToPlayer;
        Timer checkNewPlayersTimer{std::chrono::milliseconds(1000)};
        std::deque<std::string> names = {"progress_bar/progress_bar_front_3.png",
                                         "progress_bar/progress_bar_front_2.png",
                                         "progress_bar/progress_bar_front.png"};
        std::string back =  "progress_bar/progress_bar_back.png";
        sf::Vector2f size = {500, 40};
        float padding = {5};

    public:
        HealthBarHolder(GameObject& object);

        void start() override;

        void update() override;

        void checkNewPlayer();

        bool isPlayerTagged(const std::string& name);

        std::shared_ptr<GameObject> initHealthBar(const std::string& name);

        void updateBars();
    };

}