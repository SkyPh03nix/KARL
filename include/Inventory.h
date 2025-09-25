#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include "Item.h"
#include "Hotbar.h"

class Inventory {
public:
    Hotbar hotbar;

    Inventory(const sf::Vector2u& windowSize, unsigned int cap = 64);

    bool addItem(const Item& item);
    bool removeItem(const Type& type, int quantity);
    Item* getItem(const std::string& itemName);
    std::vector<std::unique_ptr<Item>>& getItems() {return items; }
    void clear() { items.clear(); }

    void toggleVisibility() { visible = !visible; }
    bool isVisible() const { return visible; }

    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);
    bool containsPoint(const sf::Vector2f& point) const {return visible && background.getGlobalBounds().contains(point);}

    bool addResource(const std::string& name, int quantity);
    int getResourceQuantity(const Type& type) const;
    unsigned int getCapacity() const { return capacity; }
    unsigned int getStackLimit(Type type) const;

    sf::FloatRect getGlobalBounds() const {return background.getGlobalBounds();}

private:
    std::vector<std::unique_ptr<Item>> items;
    unsigned int capacity = 64;
    bool visible = false; // Inventory UI visibility
    sf::RectangleShape background;
    sf::Font font;
};