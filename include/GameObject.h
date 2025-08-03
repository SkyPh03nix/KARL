#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//TODO fix includes

class GameObject{
    public:
        //virtual void update(const sf::RenderWindow& window) = 0;
        virtual void update(float deltaTime, const sf::RenderWindow& window) {}
        virtual void draw(sf::RenderWindow& window) = 0;
        //virtual void handleEvent(const sf::Event& event) {}
        virtual ~GameObject() = default;
};