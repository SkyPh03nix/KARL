#include "Sapling.h"

Sapling::Sapling(const sf::Vector2f& pos, const sf::Texture& spritesheet, int growthState)
    : spritesheet(spritesheet), position(pos), growthState(growthState)
{
    sprite.setTexture(spritesheet);
    updateSprite();
    sprite.setPosition(position);
    sprite.setOrigin(sprite.getTextureRect().width/2.f, sprite.getTextureRect().height);
}

void Sapling::update(float deltaTime, const sf::RenderWindow&) {
    if (growthState < 3) {
        growthTimer += deltaTime;
        if (growthTimer >= growthInterval) {
            growthTimer = 0.f;
            setGrowthState(growthState + 1);
        }
    }
}

void Sapling::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Sapling::setGrowthState(int state) {
    growthState = state;
    updateSprite();
}

void Sapling::updateSprite() {
    int textureSize = 128;
    int frame = std::min(growthState, 2);
    sprite.setTextureRect(sf::IntRect(frame * textureSize, 0, textureSize, textureSize));
    sprite.setScale(3.f, 3.f); 
}