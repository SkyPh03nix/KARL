#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Player;

class Entity {
    protected:
        sf::RectangleShape shape;

    public:
        Entity(sf::Vector2f size, sf::Vector2f pos, sf::Color fill = sf::Color::White);

        virtual void draw(sf::RenderWindow& window) const;
        //virtual void handleCollision(Player& player) {}
        virtual void update(float deltaTime) = 0;
        
        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;
};

class Player : public Entity {
    private:
        float speed;
        std::string name;
        sf::Font font;
        sf::Text nameText;
    public:
        Player(sf::Vector2f size, sf::Vector2f pos, float speed,const std::string& name = "", sf::Color fill = sf::Color::Green);

        void update(float deltaTime) override;
        void update(float deltaTime, sf::Vector2u windowSize);
        void draw(sf::RenderWindow& window) const override;
        void updateNameTextPosition();
        void setColor(sf::Color col);
        sf::FloatRect getGlobalBounds() const;
};

class Button : public Entity {
    private:
        bool active;
    public:
        Button(sf::Vector2f size, sf::Vector2f pos, sf::Color fill = sf::Color::Blue);

        void update(Player& player);
        void update(float deltaTime) override{};
        bool isActive() const;
};