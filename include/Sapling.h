#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Sapling : public GameObject {
public:
    Sapling(const sf::Vector2f& pos, const sf::Texture& spritesheet, int growthState = 0);

    void update(float deltaTime, const sf::RenderWindow&) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getGlobalBounds() const override { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const override { return position; }

    int getGrowthState() const { return growthState; }
    void setGrowthState(int state);

    bool isReadyToBecomeTree() const { return growthState >= 3; }

private:
    sf::Sprite sprite;
    const sf::Texture& spritesheet;
    sf::Vector2f position;
    int growthState = 0;
    float growthTimer = 0.f;
    float growthInterval = 10.f; // z.B. 10 Sekunden pro State

    void updateSprite();
};