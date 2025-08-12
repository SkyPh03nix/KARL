#pragma once
#include "GameObject.h"
#include "AnimationManager.h"

class Portal : public GameObject {
    private: 
        AnimationManager& anims; 
        bool active = false;
        int animId = -1;
        
        sf::Sprite sprite;
        sf::Color color;        

    public:
        Portal(AnimationManager& animationManager);
        
        void setTexture(sf::Texture& texture);
        void place(const sf::Vector2f& pos, const sf::Color& col, const std::string& animationName);
        void update(float deltaTime, const sf::RenderWindow& window) override;

        bool isActive() {return active;}
        void deactivate() {active = false; animId = -1;}

        const sf::Vector2f& getPosition() const {return sprite.getPosition();}
        const sf::FloatRect getBounds() const {return sprite.getGlobalBounds();}

        void draw(sf::RenderWindow& window) {if(active)window.draw(sprite);};
};