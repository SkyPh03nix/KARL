#pragma once

#include "GameObject.h"
#include <functional>

class Button : public GameObject {
    private:
        sf::RectangleShape shape;
        sf::Text text;
        sf::Font font;
        std::function<void()> onClick;
        bool wasClickedLastFrame = false;

        bool isMouseOver(const sf::RenderWindow& window) const;

    public:
        Button(sf::Vector2f size, sf::Vector2f pos, const std::string& label = "");

        void setOnClick(std::function<void()> callback);
        void setColor(sf::Color col);

        void updateTextColor();

        //void update(const sf::RenderWindow& window) override;
        void update(float deltaTime, const sf::RenderWindow& window) override;
        void draw(sf::RenderWindow& window) override;
};