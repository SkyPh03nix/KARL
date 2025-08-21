#pragma once

#include "GameObject.h"
#include "AnimationManager.h"

class Player : public GameObject {
    private:
        sf::Sprite sprite;
        AnimationManager anims; 
        int currentAnimId = -1;
        std::string currentAnimName;

        enum class Direction {Down, Up, Left, Right};
        Direction direction = Direction::Down;
        Direction lastDirection = Direction::Down;

        float speed;
        
        void initAnimationSet(sf::Texture& texture, const std::string& prefix, int frameCount, float frameTime, int directionsCount = 4);

    public:
        Player(sf::Texture& walkTexture, sf::Texture& idleTexture, sf::Vector2f pos, float speed);

        void update(float deltaTime, const sf::RenderWindow& window) override;
        void draw(sf::RenderWindow& window) override;
        void setColor(const sf::Color& col);
        void setPosition(const sf::Vector2f& pos);
        sf::FloatRect getGlobalBounds() const;
        sf::Vector2f getPosition() const {return sprite.getPosition();}
};