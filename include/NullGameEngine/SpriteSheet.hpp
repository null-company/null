#pragma once

#include <NullGameEngine.hpp>
#include <SFML/Graphics.hpp>
#include <utility>
#include <unordered_map>

namespace null {
    // Represents a set of animations stored in a single texture
    class SpriteSheet {
    private:
//        class Animation_s {
//        public:
//            std::string name;
//            int row;
//            int start;
//            int end;
//            std::vector<sf::Vector2i> framePositions = std::vector<sf::Vector2i>();
//            Animation_s(std::string  name, int row, int start, int end):name(std::move(name)), row(row), start(start), end(end) { }
//        };
        struct Animation_s {
            std::string name;
            int row;
            int start;
            int end;
            std::vector<sf::Vector2i> framePositions = std::vector<sf::Vector2i>();
        };

    public:
        std::string textureName;
        sf::Vector2i spriteSize;
        std::unordered_map<std::string, Animation_s> animations;
        Animation_s* currAnimation = nullptr;
        int currFrame = 0;

        void setAnimation(const std::string&);
        void setFrame(int);
        void addAnimation(const Animation_s&);
        void splitTextureIntoAnimations(std::vector<Animation_s>&) const;
        SpriteSheet(std::string , sf::Vector2i, std::vector<Animation_s>);

        sf::IntRect getCurrentTextureRect() const;

        friend Animation;
    };
}
