#pragma once

#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <vector>
#include <memory>
#include "Tree.h"
#include "Item.h"
#include "PortalSystem.h"
#include "RecourceManager.h"

class Game {
    public:
        Game();
        void run();
        void dropItem(const std::string& itemName, const std::string& description, 
                      Type itemType, const std::string& textureName, 
                      const sf::Vector2f& position, int quantity);
    private:
        sf::RenderWindow window;
        sf::Clock clock;
        sf::View camera; //camera centered on player
        sf::View uiView; //view for UI elements (fixed to screen)

        ResourceManager resources;

        sf::Sprite backgroundSprite;
       
        PortalSystem portals;

        std::vector<std::unique_ptr<Tree>> visibleTrees; 
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        std::vector<std::unique_ptr<DroppedItem>> droppedItems;

        void initWindow();
        void initObjects();
        void initBackground();
        void initTextures();
        void initTrees();
        void checkItemPickup();
        void loadTreesFromFile(const std::string& filename = "gamesave.txt");
        void saveTreesToFile(const std::string& filename = "gamesave.txt");

        void processEvents();
        void update(float deltaTime);
        void render();
    
};