#include "Inventory.h"
#include <iostream>

Inventory::Inventory(unsigned int cap) : capacity(cap) {
    background.setSize(sf::Vector2f(400.f, 300.f));
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setOutlineColor(sf::Color(100,100,100));
    background.setOutlineThickness(2.f);
    background.setPosition(50.f, 50.f);

    if (!font.loadFromFile("assets/used/arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
}

bool Inventory::addItem(const Item& item) {
    int stackLimit = getStackLimit(item.getType());
    int quantityToAdd = item.getQuantity();

    // try stacking first
    for (auto& invItem : items) {
        if (invItem->getType() == item.getType() && invItem->getQuantity() < stackLimit) {
            int space = stackLimit - invItem->getQuantity();
            int add = std::min(space, quantityToAdd);
            invItem->addQuantity(add);
            quantityToAdd -= add;
            if (quantityToAdd <= 0) return true;
        }
    }
    
    // new slots if inventory not full
    while (quantityToAdd > 0 && items.size() < capacity) {
        int add = std::min(stackLimit, quantityToAdd);
        items.push_back(std::make_unique<Item>(
            item.getName(),
            item.getType(),
            add,
            item.isStackable(),
            item.getTexture(),
            item.getPosition()
        ));
        quantityToAdd -= add;
    }

    // if rest quantityToAdd > 0, inventory full
    return quantityToAdd == 0;
}

bool Inventory::removeItem(const Type& type, int quantity) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getType() == type) {
            if ((*it)->getQuantity() > quantity) {
                (*it)->addQuantity(-quantity);
                return true;
            } else if ((*it)->getQuantity() == quantity) {
                for (int h = 0; h < hotbar.getSlotCount(); ++h) {
                    if (hotbar.getSlot(h) == it->get()) {
                        hotbar.setSlot(h, nullptr);
                    }
                }
                items.erase(it);
                return true;
            } else {
                return false; // Not enough items available
            }
        }
    }
    return false; // Item not found
}

Item* Inventory::getItem(const std::string& itemName) {
    for (auto& item : items) {
        if (item->getName() == itemName) {
            return item.get();
        }
    }
    return nullptr;
}

void Inventory::draw(sf::RenderWindow& window) {
    if (!visible) return;

    window.draw(background);

    float padding = 10.f;
    float itemSize = 50.f;
    float x = background.getPosition().x + padding;
    float y = background.getPosition().y + padding;

    for (const auto& item : items) {
        sf::RectangleShape itemBox(sf::Vector2f(itemSize, itemSize));
        itemBox.setPosition(x, y);
        itemBox.setFillColor(sf::Color(100, 100, 100));
        itemBox.setOutlineColor(sf::Color::White);
        itemBox.setOutlineThickness(1.f);
        window.draw(itemBox);

        if (item->getTexture()) {
            sf::Sprite sprite;
            sprite.setTexture(*item->getTexture());

             if (item->getType() == Type::SAPLING) {
                sprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
                
                float scale = ((itemSize - 10.f) / 128.f) * 3.f;
                sprite.setScale(scale, scale);
                sprite.setOrigin(128.f / 2.f, 128.f); 
                sprite.setPosition(x + itemSize / 2.f, y + itemSize); 
            } else {
                sprite.setScale((itemSize - 10.f) / item->getTexture()->getSize().x,
                                (itemSize - 10.f) / item->getTexture()->getSize().y);
                sprite.setOrigin(0.f, 0.f); 
                sprite.setPosition(x + 5.f, y + 5.f);
            }
            window.draw(sprite);
        }

        if (item->getQuantity() > 1) {
            sf::Text qtyText;
            qtyText.setFont(font);
            qtyText.setString(std::to_string(item->getQuantity()));
            qtyText.setCharacterSize(14);
            qtyText.setFillColor(sf::Color::White);
            qtyText.setPosition(x + itemSize - 15.f, y + itemSize - 20.f);
            window.draw(qtyText);
        }

        x += itemSize + padding;
        if (x + itemSize > background.getPosition().x + background.getSize().x) {
            x = background.getPosition().x + padding;
            y += itemSize + padding;
        }
    }
}

int Inventory::getResourceQuantity(const Type& type) const {
    int total = 0;
    for (const auto& item : items) {
        if (item->getType() == type) {
            total += item->getQuantity();
        }
    }
    return total;
}

unsigned int Inventory::getStackLimit(Type type) const {
    // Tools not stackable, everything else stackable to 64
    switch(type) {
        // TODO cases where stack size is different for example tools
        default:
            return 64;
    }
}

void Inventory::handleClick(const sf::Vector2f& mousePos) {
    if (!visible) return;
    //TODO
    /*
    if  (!background.getGlobalBounds().contains(mousePos)) return;

    float padding = 10.f;
    float itemSize = 50.f;
    float x = background.getPosition().x + padding;
    float y = background.getPosition().y + padding;

    for (auto& item : items) {
        sf::FloatRect itemBox(x, y, itemSize, itemSize);
        if (itemBox.contains(mousePos)) {
            return;
        }
        //TODO interact with item (e.g., use or drop)
        x += itemSize + padding;
        if (x + itemSize > background.getPosition().x + background.getSize().x) {
            x = background.getPosition().x + padding;
            y += itemSize + padding;
        }
    }*/
}
    