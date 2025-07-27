#pragma once

#include "Player.h"

Player::Player(sf::Vector2f size, sf::Vector2f pos, float speed, const std::string& name, sf::Color fill)
    : Entity(size, pos, fill), speed(speed), name(name) {
        if (!font.loadFromFile("assets/arial.ttf")) {
            std::cerr << "Fehler: Schriftart konnte nicht geladen werden!" << std::endl;
        }
    nameText.setFont(font);
    nameText.setCharacterSize(15);
    nameText.setFillColor(sf::Color::White);
    nameText.setString(name);

    sf::FloatRect textBounds = nameText.getLocalBounds();
    nameText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    nameText.setPosition(
        shape.getPosition().x + shape.getSize().x / 2.f,
        shape.getPosition().y + shape.getSize().y / 2.f
    );
}


void Player::update(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * deltaTime;
    shape.move(movement);
}

void Player::updateNameTextPosition() {
    sf::FloatRect textBounds = nameText.getLocalBounds();
    nameText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    nameText.setPosition(
        shape.getPosition().x + shape.getSize().x / 2.f,
        shape.getPosition().y + shape.getSize().y / 2.f
    );
}

void Player::update(float deltaTime, sf::Vector2u windowSize) {
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * deltaTime;
    shape.move(movement);

    // checking for wall collision and negating the movement 
    sf::FloatRect bounds = shape.getGlobalBounds();
    if (bounds.left < 0) {shape.setPosition(0,bounds.top);}
    if (bounds.left + bounds.width > windowSize.x) {shape.setPosition(windowSize.x-bounds.width,bounds.top);}
    if (bounds.top < 0) {shape.setPosition(bounds.left, 0);}
    if (bounds.top + bounds.height > windowSize.y) {shape.setPosition(bounds.left, windowSize.y - bounds.height);}

    updateNameTextPosition();
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    if(!name.empty()) {
        window.draw(nameText);
    }
}

void Player::setColor(sf::Color col) {
    shape.setFillColor(col);
}

sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}