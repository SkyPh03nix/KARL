#pragma once
#include "GameObject.h"
#include "AnimationManager.h"

class Portal : public GameObject {
    private: 
        //sf::RectangleShape shape; //TODO change to sprite later
        AnimationManager& anims; 
        bool active = false;
        std::string currentAnimation;
        sf::Sprite sprite;
        sf::Color color;        

    public:
        Portal(AnimationManager& animationManager);
        
        void setTexture(sf::Texture& texture);
        void place(const sf::Vector2f& pos, const sf::Color& col, const std::string& animationName);
        void setAnimation(const std::string& animName) {currentAnimation=animName;}
        void update(float deltaTime, const sf::RenderWindow& window) override;

        bool isActive() {return active;}
        void deactivate() {active = false;}

        const sf::Vector2f& getPosition() const {return sprite.getPosition();}
        const sf::FloatRect getBounds() const {return sprite.getGlobalBounds();}

        void draw(sf::RenderWindow& window) {if(active)window.draw(sprite);};
        void updateFrame(const sf::IntRect& frame); //TODO update
};