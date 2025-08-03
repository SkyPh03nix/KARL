#pragma once

#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include "Player.h"
#include "Button.h"
#include <vector>
#include <memory>
//TODO fix includes

class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window;
        sf::Clock clock;

        //std::vector<GameObject> gameObjects;
        std::vector<std::unique_ptr<GameObject>> gameObjects;

        //Player player;
        //std::vector<Button> buttons;

        void initWindow();
        void initObjects();

        void processEvents();
        void update(float deltaTime);
        void render();
    
};