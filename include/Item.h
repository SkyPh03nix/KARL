#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <functional>
#include "GameObject.h"

enum class Type {
    WOOD, STONE, APPLE
};

class Item {
public:
    Item(const std::string& name,Type type, int quantity, bool stackable, const sf::Texture* texture,const sf::Vector2f& position)
    : name(name), type(type), quantity(quantity),
      stackable(stackable), texture(texture) {
        if (texture) sprite.setTexture(*texture);
        sprite.setPosition(position);
    }

    void update(float deltaTime, const sf::RenderWindow& window) {} // TODO animation
    void draw(sf::RenderWindow& window) { window.draw(sprite); }

    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    //void setUseEffect(std::function<void()> effect) {useEffect = effect;}
    //void use() { if(useEffect) useEffect(); }

    const std::string& getName() const {return name;}
    Type getType() const {return type;}
    const sf::Texture* getTexture() const {return texture;}
    int getQuantity() const {return quantity;}
    //void setQuantity(int qty) {quantity = qty;}
    void addQuantity(int qty) {quantity += qty;}
    bool isStackable() const {return stackable;}


private:
    std::string name;
    Type type;
    int quantity;
    bool stackable;

    sf::Sprite sprite;
    const sf::Texture* texture;
    //std::function<void()> useEffect; // Function to define the effect when the item is used
};

/*
class DroppedItem :public GameObject {
private:
    Item item;
    sf::Sprite sprite;
    bool pickedUp = false;
    float pickingRadius = 30.f;

    public:
    DroppedItem(const Item& item, const sf::Vector2f& position)
        : item(item) {
        if (item.getTexture()) {
            sprite.setTexture(*item.getTexture());
            //sprite.setScale(1.f, 1.f);
            sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);
        }
            
        sprite.setPosition(position);
    }

    void update(float deltaTime, const sf::RenderWindow& window) override {
        if (pickedUp) return;

        sprite.setPosition(sprite.getPosition().x , sprite.getPosition().y + std::sin(deltaTime) * 10.f); //TODO check if correct
    }

    void draw(sf::RenderWindow& window) override {
        if (!pickedUp)
            window.draw(sprite);
    }

    bool checkPickup(const sf::Vector2f& playerPos) {
        if (pickedUp) return false;

        float distance = std::hypot(playerPos.x - sprite.getPosition().x, playerPos.y - sprite.getPosition().y);
        if (distance <= pickingRadius) {
            pickedUp = true;
            return true;
        }
        return false;
    }
    const Item& getItem() const { return item; }
    bool isPickedUp() const { return pickedUp; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
};
*/