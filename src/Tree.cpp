#include "Tree.h"
#include "RecourceManager.h"
#include "Game.h"

Tree::Tree(const sf::Vector2f& pos, const sf::Texture& treeTexture, const sf::Texture& choppedTexture, const sf::Texture& woodTexture, const sf::Texture& saplingTexture, const sf::Texture& appleTexture)
    : treeTexture(treeTexture), choppedTexture(choppedTexture), woodTexture(woodTexture), saplingTexture(saplingTexture), appleTexture(appleTexture),
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

void Tree::chop(std::vector<std::unique_ptr<Item>>& worldItems) {
    if (!chopped) {
        chopped = true; 
        respawnTimer = 0.f;

        worldItems.push_back(std::make_unique<Item>("Wood", Type::WOOD, 1, true, &woodTexture, position));
        if(rand() % 100 < 30) { // 30% chance to drop a sapling
            worldItems.push_back(std::make_unique<Item>("Sapling", Type::SAPLING, 1, true, &saplingTexture, position + sf::Vector2f(10.f, 0.f)));
        }
        if(rand() % 100 < 20) { // 20% chance to drop an apple
            worldItems.push_back(std::make_unique<Item>("Apple", Type::APPLE, 1, true, &appleTexture, position + sf::Vector2f(10.f, 0.f)));
        }
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

sf::FloatRect Tree::getGlobalBounds() const {
    if (chopped)  {
        return sf::FloatRect(0,0,0,0); // No collision when chopped
    }
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float trunkWidth  = bounds.width * 0.1f;
    float trunkHeight = bounds.height * 0.1f;
    float trunkX = bounds.left + (bounds.width - trunkWidth) / 2.f;
    float trunkY = bounds.top + (bounds.height - trunkHeight -10);

    return sf::FloatRect(trunkX, trunkY, trunkWidth, trunkHeight); 
}