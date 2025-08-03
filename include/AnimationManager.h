#pragma once

#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class AnimationManager {
    private:
        struct AnimData {
            sf::Texture* texture = nullptr;
            Animation animation;
        };
        std::unordered_map<std::string, AnimData> animations;
        std::string currentAnimation;
    public:
        void addAnimation(const std::string& name, sf::Texture& texture, const std::vector<sf::IntRect>& frames, float frameTime, bool loop);
        void play(const std::string& name);
        void update(float deltaTime);
        void applyToSprite(sf::Sprite& sprite);
};