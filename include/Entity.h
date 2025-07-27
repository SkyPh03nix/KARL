#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Player;

class Entity {
    protected:
        sf::RectangleShape shape;

    public:
        Entity(sf::Vector2f size, sf::Vector2f pos, sf::Color fill = sf::Color::White);

        virtual void draw(sf::RenderWindow& window) const;
        virtual void handleCollision(Player& player) {}
        virtual void update(float deltaTime) = 0;
        
        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;
};