#pragma once

#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <vector>
#include <memory>
#include "PortalSystem.h"
//TODO fix includes

class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window;
        sf::Clock clock;

        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        sf::Texture playerWalkTexture;
        sf::Texture playerIdleTexture;

        PortalSystem portals;
        
        std::vector<std::unique_ptr<GameObject>> gameObjects;


        void initWindow();
        void initObjects();
        void initBackground();
        void initPlayerTexture();

        void processEvents();
        void update(float deltaTime);
        void render();
    
};