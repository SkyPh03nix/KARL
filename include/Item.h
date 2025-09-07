#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <functional>

enum class Type {
    WEAPON,
    ARMOR,
    CONSUMABLE,
    RESOURCE
};

class Item {
public:
    Item(const std::string& name, const std::string description, Type type, sf::Texture* texture, int quantity = 1)
        : name(name), description(description), type(type), texture(texture), quantity(quantity) {}

    void setUseEffect(std::function<void()> effect) {useEffect = effect;}
    void use() { if(useEffect) useEffect(); }

    const std::string& getName() const {return name;}
    const std::string& getDescription() const {return description;}
    Type getType() const {return type;}
    sf::Texture* getTexture() const {return texture;}
    int getQuantity() const {return quantity;}
    void setQuantity(int qty) {quantity = qty;}
    void addQuantity(int qty) {quantity += qty;}

    bool isStackable() const {return type == Type::CONSUMABLE || type == Type::RESOURCE;}


private:
    std::string name;
    std::string description;
    Type type;
    sf::Texture* texture;
    int quantity;
    std::function<void()> useEffect; // Function to define the effect when the item is used
};