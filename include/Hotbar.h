#pragma once

#include <vector>
#include "Item.h"

class Hotbar {
public:
    Hotbar() {slots.resize(10, nullptr);} // 10 empty slots
    void setSlot(int index, Item* item); //pointer to inventory item
    Item* getSlot(int index) const;
    void draw(sf::RenderWindow& window, float startX, float startY);
    int getSlotCount() const { return static_cast<int>(slots.size()); }
private:
    std::vector<Item*> slots; // item pointers
};