#pragma once

#include <NullGameEngine.hpp>
#include <SFML/Graphics.hpp>
#include <utility>

namespace null {
    class SpriteSheet;

    class SpriteSheet {
    private:
        class Animation_c {
        public:
            std::string name;
            int row;
            int start;
            int end;
            std::vector<sf::Vector2i> framePositions = std::vector<sf::Vector2i>();
            Animation_c(std::string  name, int row, int start, int end):name(std::move(name)), row(row), start(start), end(end) { }
        };

    public:
        std::string textureName;
        sf::Vector2i spriteSize;
        std::unordered_map<std::string, Animation_c> animations;
        Animation_c* currAnimation = nullptr;
        int currFrame = 0;

        void setAnimation(const std::string&);
        void setFrame(int);
        void addAnimation(const Animation_c&);
        void splitTextureIntoAnimations(std::vector<Animation_c>&) const;
        SpriteSheet(std::string , sf::Vector2i, std::vector<Animation_c>);

        sf::IntRect getCurrentTextureRect() const;
    };
}