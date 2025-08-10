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

        struct AnimationInstance {
            std::string animationName;
            Animation animation;
        };

        std::unordered_map<std::string, AnimData> animations;
        std::unordered_map<int, AnimationInstance> activeAnimations;
        std::string currentAnimation;

        int nextId = 0;
    public:
        void addAnimation(const std::string& name, sf::Texture& texture, const std::vector<sf::IntRect>& frames, float frameTime, bool loop);
        int play(const std::string& name);
        void update(float deltaTime);
        void applyToSprite(int id, sf::Sprite& sprite);
        sf::IntRect getCurrentFrame(int id) const;
};