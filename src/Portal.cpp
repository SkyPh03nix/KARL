#include "Portal.h"
#include <iostream>


Portal::Portal(AnimationManager& animationManager)
    : anims(animationManager), active(false), currentAnimation("") {
}

void Portal::setTexture(sf::Texture& portalTexture) {
    sprite.setTexture(portalTexture);
}

void Portal::place(const sf::Vector2f& pos, const sf::Color& col, const std::string& animationName) {
    sprite.setPosition(pos);
    active=true;
    color = col;

    currentAnimation = animationName;
    anims.play(currentAnimation);
} 

void Portal::updateFrame(const sf::IntRect& frame) {
    sprite.setTextureRect(frame);
}

void Portal::update(float deltaTime, const sf::RenderWindow& window) {
    if (!active) return;

    anims.update(deltaTime);

    // Hole den aktuellen Frame der Animation und setze das Sprite-TextureRect
    sf::IntRect frame = anims.getCurrentFrame(currentAnimation);
    sprite.setTextureRect(frame);
}