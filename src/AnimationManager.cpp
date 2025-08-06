#include "AnimationManager.h"
#include <iostream>
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

void AnimationManager::addAnimation(const std::string& name, sf::Texture& texture, const std::vector<sf::IntRect>& frames, float frameTime, bool loop) {
    std::cout << "Add Animation: " << name << " mit " << frames.size() << " Frames\n";
    animations[name] = AnimData{ &texture, Animation(frames, frameTime, loop) };
}
        
void AnimationManager::play(const std::string& name) {
    if (currentAnimation != name && animations.find(name) != animations.end()) {
        currentAnimation = name;
        animations[currentAnimation].animation.stop();
        animations[currentAnimation].animation.play();
    }
}
        
void AnimationManager::update(float deltaTime) {
    if (currentAnimation.empty()) return;
    animations[currentAnimation].animation.update(deltaTime);
}


void AnimationManager::applyToSprite(sf::Sprite& sprite) {
    if (currentAnimation.empty()) {
        std::cerr << "AnimationManager::applyToSprite: currentAnimation leer\n";
        return;
    }

    auto it = animations.find(currentAnimation);
    if (it == animations.end()) {
        std::cerr << "AnimationManager::applyToSprite: Animation nicht gefunden\n";
        return;
    }

    const AnimData& animData = it->second;

    if (!animData.texture) {
        std::cerr << "AnimationManager::applyToSprite: Textur ist nullptr\n";
        return;
    }

    sprite.setTexture(*animData.texture);
    sprite.setTextureRect(animData.animation.getCurrentFrame());
}


