#pragma once

#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <vector>
#include <memory>
#include "Tree.h"
#include "PortalSystem.h"
#include "RecourceManager.h"

class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window;
        sf::Clock clock;
        sf::View camera;

        ResourceManager resources;

        sf::Sprite backgroundSprite;
       
        PortalSystem portals;

        std::vector<std::unique_ptr<Tree>> visibleTrees;
        
        std::vector<std::unique_ptr<GameObject>> gameObjects;

        void initWindow();
        void initObjects();
        void initBackground();
        void initTextures();
        void initTrees();

        void processEvents();
        void update(float deltaTime);
        void render();
    
};