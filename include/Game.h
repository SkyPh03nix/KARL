#pragma once

#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <vector>
#include <memory>
#include "PortalSystem.h"
#include "RecourceManager.h"
//TODO fix includes

class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window;
        sf::Clock clock;

        ResourceManager resources;

        sf::Sprite backgroundSprite;
       
        PortalSystem portals;
        
        std::vector<std::unique_ptr<GameObject>> gameObjects;

        void initWindow();
        void initObjects();
        void initBackground();
        void initTextures();

        void processEvents();
        void update(float deltaTime);
        void render();
    
};