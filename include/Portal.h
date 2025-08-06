#pragma once
#include "GameObject.h"

class Portal : public GameObject {
    private: 
        sf::RectangleShape shape; //TODO change to sprite later
        bool active = false;

    public:
        void place(const sf::Vector2f& pos, const sf::Color& col) {
            shape.setSize({64.f,64.f});
            shape.setPosition(pos);
            shape.setFillColor(col);
            active=true;
        } 

        bool isActive() {return active;}
        void deactivate() {active = false;}

        const sf::Vector2f& getPosition() const {return shape.getPosition();}
        const sf::FloatRect getBounds() const {return shape.getGlobalBounds();}

        void draw(sf::RenderWindow& window){if(active)window.draw(shape);};
        void update(float deltaTime, const sf::RenderWindow& window) {}
};