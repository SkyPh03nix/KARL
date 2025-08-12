#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class GameObject{
    public:
        virtual void update(float deltaTime, const sf::RenderWindow& window) {}
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual ~GameObject() = default;
};