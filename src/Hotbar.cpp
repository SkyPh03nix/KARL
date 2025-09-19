#include "Hotbar.h"
#include <SFML/Graphics.hpp>

void Hotbar::setSlot(int index, Item* item) {
    if (index >= 0 && index <static_cast<int>(slots.size())) {
        slots[index] = item;
    }
}

Item* Hotbar::getSlot(int index) const {
    if (index >= 0 && index <static_cast<int>(slots.size())) {
        return slots[index];
    }
    return nullptr;
}

void Hotbar::draw(sf::RenderWindow& window, float startX, float startY) {
    float slotSize = 48.f;
    float spacing = 8.f;

    sf::RectangleShape slotRect(sf::Vector2f(slotSize, slotSize));
    slotRect.setFillColor(sf::Color(80, 80, 80, 200));
    slotRect.setOutlineColor(sf::Color::White);
    slotRect.setOutlineThickness(2.f);

    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    for (size_t i = 0; i < slots.size(); ++i) {
        slotRect.setPosition(startX + i * (slotSize + spacing), startY);
        window.draw(slotRect);

        if (slots[i]) {
            sf::Sprite sprite;
            if (slots[i]->getTexture()) {
                sprite.setTexture(*slots[i]->getTexture());
                sprite.setScale(slotSize / sprite.getLocalBounds().width, slotSize / sprite.getLocalBounds().height);
                sprite.setPosition(slotRect.getPosition());
                window.draw(sprite);
            }

            sf::Text amountText;
            amountText.setFont(font);
            amountText.setCharacterSize(14);
            amountText.setFillColor(sf::Color::White);
            amountText.setString(std::to_string(slots[i]->getQuantity()));
            amountText.setPosition(slotRect.getPosition().x + slotSize - amountText.getLocalBounds().width - 4,
                                   slotRect.getPosition().y + slotSize - amountText.getLocalBounds().height - 4);
            window.draw(amountText);
        }
    }
}