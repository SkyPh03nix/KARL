#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GameObject.h>
//TODO fix includes

class Game {
    public:
        Game();

        void run();
    private:
        sf::RenderWindow window;
        std::vector<GameObject> gameObjects;
        sf::Clock clock;

        void processEvents();
        void update(float deltaTime);
        void render();
        

};