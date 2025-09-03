#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Tree: public GameObject {
    public:
        Tree(const sf::Vector2f& pos, const sf::Texture& texture)
            : position(pos), sprite(texture){
            sprite.setPosition(position);
            sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y); // Origin at bottom center
        }

        void update(float, const sf::RenderWindow&) override {}
        void draw(sf::RenderWindow& window) override {window.draw(sprite);}
        sf::Vector2f getPosition() const { return position; }

    private:
        sf::Vector2f position;
        sf::Sprite sprite;
};