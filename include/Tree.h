#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Tree: public GameObject {
    public:
        Tree(const sf::Vector2f& pos, const sf::Texture& texture);

        void update(float, const sf::RenderWindow&) override {}
        void draw(sf::RenderWindow& window) override {window.draw(sprite);}

        sf::Vector2f getPosition() const {return position;}
        sf::FloatRect getBounds() const;

        void setScale(float x, float y) {sprite.setScale(x, y);}

    private:
        sf::Vector2f position;
        sf::Sprite sprite;
};