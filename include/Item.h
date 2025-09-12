#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <functional>
#include "GameObject.h"

enum class Type {
    WOOD, STONE, APPLE, SAPLING
};

class Item {
public:
    Item(const std::string& name,Type type, int quantity, bool stackable, const sf::Texture* texture,const sf::Vector2f& position)
    : name(name), type(type), quantity(quantity),
      stackable(stackable), texture(texture) {
        if (texture) sprite.setTexture(*texture);
        sprite.setOrigin(texture->getSize().x/2.0f, texture->getSize().y/2.0f);
        sprite.setScale(2.0f,2.0f);
        sprite.setPosition(position);
        
    }

    void update(float deltaTime, const sf::RenderWindow& window) {} // TODO animation
    void draw(sf::RenderWindow& window) { window.draw(sprite); }

    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    const std::string& getName() const {return name;}
    Type getType() const {return type;}
    const sf::Texture* getTexture() const {return texture;}
    int getQuantity() const {return quantity;}
    void addQuantity(int qty) {quantity += qty;}
    bool isStackable() const {return stackable;}


private:
    std::string name;
    Type type;
    int quantity;
    bool stackable;

    sf::Sprite sprite;
    const sf::Texture* texture;
};