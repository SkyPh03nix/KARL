#include "Button.h"
#include "Player.h"

Button::Button(sf::Vector2f size, sf::Vector2f pos, sf::Color fill) 
    : Entity(size,pos, fill), active(false){
}

void Button::update(Player& player) {
    if (shape.getGlobalBounds().intersects(player.getGlobalBounds())) {
        active = true;
        player.setColor(sf::Color::Magenta);
    } else {
        active = false;
        player.setColor(sf::Color::Blue);

    }
}

bool Button::isActive() const {
    return active;
}