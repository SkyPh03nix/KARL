#include "Player.h"
#include "GameObject.h"
#include "Utils.h"
#include <AnimationManager.h>


Player::Player(sf::Texture& walkTexture,sf::Texture& idleTexture, sf::Vector2f pos, float speed)
    : speed(speed){

    // init animations
    initAnimationSet(walkTexture, "walk", 8, 0.1f);
    initAnimationSet(idleTexture, "idle", 6, 0.15f);

    // set initial texture and sprite
    sprite.setTexture(idleTexture);
    sprite.setPosition(pos);
    sprite.setScale(4.f,4.f);
    
    anims.play("idle_down");
}

void Player::initAnimationSet(sf::Texture& texture, const std::string& prefix, int frameCount, float frameTime, int directionsCount) {
    texture.setSmooth(false); // ! important for pixel art

    const int frameWidth = 64;
    const int frameHeight = 64;

    for (int dir = 0; dir < directionsCount; ++dir) {
        std::vector<sf::IntRect> frames;
        for (int i = 0; i < frameCount; ++i) {
            frames.emplace_back(i * frameWidth, dir * frameHeight, frameWidth, frameHeight);
        }
        std::string animName = prefix + "_";
        switch (dir) {
            case 0: animName += "down"; break;
            case 1: animName += "up"; break;
            case 2: animName += "left"; break;
            case 3: animName += "right"; break;
            default: animName += "unknown"; break;
        }
        anims.addAnimation(animName, texture, frames, frameTime, true);
    }
}

void Player::update(float deltaTime, const sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * deltaTime;
    sprite.move(movement);

    if (movement.x != 0.f || movement.y != 0.f) {
        // Walk Animation
        if (std::abs(movement.x) > std::abs(movement.y)) {
            if (movement.x > 0.f) {
                anims.play("walk_right"); lastDirection = Direction::Right;
            } else {
                anims.play("walk_left"); lastDirection = Direction::Left;
            }
        } else {
            if (movement.y > 0.f) {
                anims.play("walk_down"); lastDirection = Direction::Down;
            } else {
                anims.play("walk_up"); lastDirection = Direction::Up;
            }
        }
    } else {
        // Idle Animation
        switch (lastDirection) {
            case Direction::Down:  anims.play("idle_down"); break;
            case Direction::Left:  anims.play("idle_left"); break;
            case Direction::Right: anims.play("idle_right"); break;
            case Direction::Up:    anims.play("idle_up"); break;
        }
    }

    //update animation
    anims.update(deltaTime);
    anims.applyToSprite(sprite);

    //Check for window border
    sf::FloatRect bounds = getGlobalBounds();
    sf::Vector2f pos = sprite.getPosition();

    if (bounds.left < 0) pos.x -= bounds.left; // Left
    if (bounds.top < 0) pos.y -= bounds.top;   // Top
    if (bounds.left + bounds.width > windowSize.x) pos.x -= (bounds.left + bounds.width - windowSize.x); // Right
    if (bounds.top + bounds.height > windowSize.y) pos.y -= (bounds.top + bounds.height - windowSize.y); // Bottom
    sprite.setPosition(pos);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::setColor(const sf::Color& col) {
    sprite.setColor(col);
}

sf::FloatRect Player::getGlobalBounds() const {
    // 32x32 Box in middle of sprite //TODO look for better value here (just a guess rn)
    float boxSize = 32.f;
    float halfBox = boxSize / 2.f;

    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    float centerX = spriteBounds.left + spriteBounds.width / 2.f;
    float centerY = spriteBounds.top + spriteBounds.height / 2.f;

    return sf::FloatRect(centerX - halfBox, centerY - halfBox, boxSize, boxSize);
    
   //return sprite.getGlobalBounds(); //old version gives uncentered player bounds
}

void Player::setPosition(const sf::Vector2f& pos) {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sprite.setPosition(pos.x - bounds.width / 2.f, pos.y - bounds.height / 2.f); //sets position with center offset
}