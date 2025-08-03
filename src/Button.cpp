#include "Button.h"
#include "Player.h"
#include <iostream>
#include "Utils.h"


Button::Button(sf::Vector2f size, sf::Vector2f pos, const std::string& label) {
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Cyan);

    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler: Schriftart konnte nicht geladen werden!" << std::endl;
        exit(1);
    }
    text.setFont(font);
    text.setString(label);
    //text.setStyle(sf::Text::Bold);
    text.setCharacterSize(14);

    // Position Text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(
        pos.x + size.x / 2.0f,
        pos.y + size.y / 2.0f
    );

    text.setFillColor(getContrastingTextColor(shape.getFillColor()));
}

void Button::setColor(sf::Color col) {
    shape.setFillColor(col);
    text.setFillColor(getContrastingTextColor(col));
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

bool Button::isMouseOver(const sf::RenderWindow& window) const {
    auto mousePos = sf::Mouse::getPosition(window);
    auto mousePosF = window.mapPixelToCoords(mousePos); //TODO was macht das??
    return shape.getGlobalBounds().contains(mousePosF);
}

void Button::update(float deltaTime, const sf::RenderWindow& window) {
    bool isHovered = isMouseOver(window);
    
    if (isHovered) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !wasClickedLastFrame && onClick) {
            onClick();
        }
        wasClickedLastFrame = isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

/*
void Button::update(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getGlobalBounds())) {
        if (!active) player.setColor(sf::Color(rand()%255, rand()%255, rand()%255));
        active = true;
        //player.setColor(sf::Color::Magenta);
    } else {
        active = false;
    }
}



bool Button::isActive() const {
    return active;
}

*/