#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>

class Player : public GameObject {
    private:
        sf::RectangleShape shape;
        float speed;
        std::string name;

        sf::Text nameText;
        sf::Font font;

    public:
        Player(sf::Vector2f size, sf::Vector2f pos, float speed, const std::string& name = "", sf::Color fill = sf::Color::Green);

        void update(float deltaTime, const sf::RenderWindow& window) override;
        void draw(sf::RenderWindow& window) override;
        void updateNameTextPosition();
        void setColor(const sf::Color& col);
        sf::FloatRect getGlobalBounds() const;
};