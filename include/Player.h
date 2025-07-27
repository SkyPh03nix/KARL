#pragma once

#include "Entity.h"

class Player : public Entity {
    private:
        float speed;
        std::string name;
        sf::Font font;
        sf::Text nameText;
    public:
        Player(sf::Vector2f size, sf::Vector2f pos, float speed,const std::string& name = "", sf::Color fill = sf::Color::Green);

        void update(float deltaTime) override;
        void update(float deltaTime, sf::Vector2u windowSize);
        void draw(sf::RenderWindow& window) const override;
        void updateNameTextPosition();
        void setColor(sf::Color col);
        sf::FloatRect getGlobalBounds() const;
};