#include "Tree.h"

Tree::Tree(const sf::Vector2f& pos, const sf::Texture& texture)
    : position(pos), sprite(texture){
    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y); // Origin at bottom center
}

sf::FloatRect Tree::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float trunkWidth  = bounds.width * 0.1f;
    float trunkHeight = bounds.height * 0.1f;
    float trunkX = bounds.left + (bounds.width - trunkWidth) / 2.f;
    float trunkY = bounds.top + (bounds.height - trunkHeight -10);

    return sf::FloatRect(trunkX, trunkY, trunkWidth, trunkHeight); 
}