#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "Item.h"
#include <memory>

class Tree: public GameObject {
    public:
        Tree(const sf::Vector2f& pos, const sf::Texture& texture, const sf::Texture& choppedTexture, const sf::Texture& woodTexture, const sf::Texture& saplingTexture, const sf::Texture& appleTexture);

        void update(float deltaTime, const sf::RenderWindow&) override;
        void draw(sf::RenderWindow& window) override;

        sf::Vector2f getPosition() const override {return position;}
        sf::FloatRect getGlobalBounds() const override;

        void chop(std::vector<std::unique_ptr<Item>>& worldItems);
        bool isChopped() const {return chopped;}

        void setScale(float x, float y);      
    private:
        sf::Sprite sprite;
        sf::Sprite choppedSprite; 
        const sf::Texture& treeTexture;
        const sf::Texture& choppedTexture;
        const sf::Texture& woodTexture;
        const sf::Texture& saplingTexture;
        const sf::Texture& appleTexture;
        
        sf::Vector2f position;
        bool chopped;
        float respawnTimer;
        const float respawnTime;
};