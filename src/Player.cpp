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
    sprite.setTexture(walkTexture);
    //sprite.setOrigin(64 / 2.f, 64 / 2.f); //centers the sprite correctly but puts it in top left corner of 
    sprite.setPosition(pos);
    sprite.setScale(4.f,4.f);
    
    anims.play("idle_down");
}

void Player::initAnimationSet(sf::Texture& texture, const std::string& prefix, int frameCount, float frameTime, int directionsCount) {
    texture.setSmooth(false);

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
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f size(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    if (pos.x < 0) pos.x = 0;
    if (pos.y < 0) pos.y = 0;
    if (pos.x + size.x > windowSize.x) pos.x = windowSize.x - size.x;
    if (pos.y + size.y > windowSize.y) pos.y = windowSize.y - size.y;

    sprite.setPosition(pos);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::setColor(const sf::Color& col) {
    sprite.setColor(col);
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void Player::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}