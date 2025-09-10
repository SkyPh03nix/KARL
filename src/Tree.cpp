#include "Tree.h"

Tree::Tree(const sf::Vector2f& pos, const sf::Texture& treeTexture, const sf::Texture& choppedTexture)
    : treeTexture(treeTexture), choppedTexture(choppedTexture),
    position(pos), chopped(false), 
    respawnTimer(0.f), respawnTime(10.f) {
    sprite.setTexture(treeTexture);
    sprite.setPosition(position);
    sprite.setOrigin(treeTexture.getSize().x / 2.0f, treeTexture.getSize().y); // Origin at bottom center

    choppedSprite.setTexture(choppedTexture);
    choppedSprite.setPosition(position);
    choppedSprite.setOrigin(choppedTexture.getSize().x / 2.0f, choppedTexture.getSize().y - 5);
}

void Tree::update(float deltaTime, const sf::RenderWindow&) {
    if (chopped) {
        respawnTimer+=deltaTime;
        if (respawnTimer >= respawnTime) {
            chopped = false;
            respawnTimer = 0.f;
        }
    }
}

void Tree::chop() {
    if (!chopped) {
        chopped = true; 
        respawnTimer = 0.f;
    }
}

void Tree::draw(sf::RenderWindow& window) {
    if(chopped) {
        window.draw(choppedSprite);
    } else {
        window.draw(sprite);
    }
}

void Tree::setScale(float x, float y) {
    sprite.setScale(x, y);
    choppedSprite.setScale(x, y);
}

sf::FloatRect Tree::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float trunkWidth  = bounds.width * 0.1f;
    float trunkHeight = bounds.height * 0.1f;
    float trunkX = bounds.left + (bounds.width - trunkWidth) / 2.f;
    float trunkY = bounds.top + (bounds.height - trunkHeight -10);

    return sf::FloatRect(trunkX, trunkY, trunkWidth, trunkHeight); 
}