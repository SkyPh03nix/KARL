#include <vector>
#include <string>
#include <map>
#include <functional>
#include "Item.h"

class Inventory {
public:

    Inventory(unsigned int cap = 64);

    bool addItem(const Item& item);
    bool removeItem(const Type& type, int quantity);
    Item* getItem(const std::string& itemName);
    const std::vector<Item>& getItems() const { return items; }
    void clear() { items.clear(); }

    void toggleVisibility() { visible = !visible; }
    bool isVisible() const { return visible; }

    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);
    bool containsPoint(const sf::Vector2f& point) const {return visible && background.getGlobalBounds().contains(point);}

    bool addResource(const std::string& name, int quantity);
    int getResourceQuantity(const Type& type) const;
private:
    std::vector<Item> items;
    unsigned int capacity = 64;
    bool visible = false; // Inventory UI visibility
    sf::RectangleShape background;
    sf::Font font;
};