#include "Portal.h"
#include <iostream>


Portal::Portal(AnimationManager& animationManager)
    : anims(animationManager), active(false){
}

void Portal::setTexture(sf::Texture& portalTexture) {
    sprite.setTexture(portalTexture);
}

void Portal::place(const sf::Vector2f& pos, const sf::Color& col, const std::string& animationName) {
    sprite.setPosition(pos);
    active=true;
    color = col;

    animId = anims.play(animationName);
}

void Portal::update(float deltaTime, const sf::RenderWindow& window) {
    if (!active || animId == -1) return;

    anims.applyToSprite(animId, sprite);
}