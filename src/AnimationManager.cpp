#include "AnimationManager.h"
#include <iostream>
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

void AnimationManager::addAnimation(const std::string& name, sf::Texture& texture, const std::vector<sf::IntRect>& frames, float frameTime, bool loop) {
    std::cout << "AnimationManager::" << __func__ << ":" << name << " mit " << frames.size() << " Frames\n";
    animations[name] = AnimData{ &texture, Animation(frames, frameTime, loop) };
}
        
int AnimationManager::play(const std::string& name) {
    auto it = animations.find(name);
    if (it == animations.end()) {
        std::cerr << "Animation '" << name << "' nicht gefunden!\n";
        return -1;
    }

    AnimationInstance instance;
    instance.animationName = name;
    instance.animation = it->second.animation; // Kopie der Vorlage
    instance.animation.play();

    int id = nextId++;
    activeAnimations[id] = instance;
    return id;
}
        
void AnimationManager::update(float deltaTime) {
    for (auto& [id, inst] : activeAnimations) {
        inst.animation.update(deltaTime);
    }
}

void AnimationManager::applyToSprite(int id, sf::Sprite& sprite) {
    auto it = activeAnimations.find(id);
    if (it == activeAnimations.end()) return;

    auto baseAnim = animations.find(it->second.animationName);
    if (baseAnim == animations.end()) return;

    sprite.setTexture(*baseAnim->second.texture);
    sprite.setTextureRect(it->second.animation.getCurrentFrame());
}

sf::IntRect AnimationManager::getCurrentFrame(int id) const {
    auto it = activeAnimations.find(id);
    if (it == activeAnimations.end()) {
        static sf::IntRect empty;
        return empty;
    }
    return it->second.animation.getCurrentFrame();
}