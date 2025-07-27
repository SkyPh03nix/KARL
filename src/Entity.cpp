#include "Entity.h"
#include <string>
#include <iostream>


// --------------------------- ENTITY ---------------------------- //

Entity::Entity(sf::Vector2f size, sf::Vector2f pos, sf::Color fill) {
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(fill);
}

void Entity::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::Vector2f Entity::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Entity::getSize() const {
    return shape.getSize();
}