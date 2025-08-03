#include "Player.h"
#include <iostream>
#include "GameObject.h"
#include "Utils.h"

Player::Player(sf::Vector2f size, sf::Vector2f pos, float speed, const std::string& name, sf::Color fill)
    : speed(speed), name(name) {
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(fill);

    //init nametag
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler: Schriftart konnte nicht geladen werden!" << std::endl;
        exit(1);
    }
    nameText.setFont(font);
    nameText.setCharacterSize(12);
    nameText.setString(name);

    sf::FloatRect textBounds = nameText.getLocalBounds();
    sf::Vector2f center = shape.getPosition() + shape.getSize() / 2.f;
    nameText.setOrigin( textBounds.left + textBounds.width / 2.0f, 
                        textBounds.top + textBounds.height / 2.0f);
    nameText.setPosition(center);

    nameText.setFillColor(getContrastingTextColor(fill));
}

void Player::updateNameTextPosition() {
    sf::Vector2f shapeCenter = shape.getPosition() + shape.getSize() / 2.f;
    nameText.setPosition(shapeCenter);
}

void Player::update(float deltaTime, const sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * deltaTime;
    shape.move(movement);

    //Check for window border
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    if (pos.x < 0) pos.x = 0;
    if (pos.y < 0) pos.y = 0;
    if (pos.x + size.x > windowSize.x) pos.x = windowSize.x - size.x;
    if (pos.y + size.y > windowSize.y) pos.y = windowSize.y - size.y;

    shape.setPosition(pos);
    //nameText.setPosition(pos.x, pos.y - 20);      // name over Player
    updateNameTextPosition();                       // name next to Player
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
    if(!name.empty()) {
        window.draw(nameText);
    }
}

void Player::setColor(const sf::Color& col) {
    shape.setFillColor(col);
    nameText.setFillColor(getContrastingTextColor(col));
}

sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}