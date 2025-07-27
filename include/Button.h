#pragma once

#include "Entity.h"

class Button : public Entity {
    private:
        bool active;
    public:
        Button(sf::Vector2f size, sf::Vector2f pos, sf::Color fill = sf::Color::Blue);

        void update(Player& player);
        void update(float deltaTime) override{};
        bool isActive() const;
};