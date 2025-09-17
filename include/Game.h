#pragma once

#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <vector>
#include <memory>
#include "Tree.h"
#include "Item.h"
#include "PortalSystem.h"
#include "RecourceManager.h"
#include "Sapling.h"

class Game {
    public:
    std::vector<std::unique_ptr<Item>> worldItems;
    ResourceManager resources;
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

        Player* player;

        sf::Sprite backgroundSprite;
       
        PortalSystem portals;

        std::vector<std::unique_ptr<Tree>> visibleTrees;
        std::vector<std::unique_ptr<Sapling>> saplings; 
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        
        void initWindow();
        void initObjects();
        void initBackground();
        void initTextures();
        void initTrees();
        void checkItemPickup();
        //void loadTreesFromFile(const std::string& filename = "gamesave.txt");
        void saveToFile(const std::string& filename = "gamesave.txt");
        void loadFromFile(const std::string& filename);
        void resolveCollisions(const sf::Vector2f& oldPos, const std::vector<GameObject*>& blockers);

        void processEvents();
        void update(float deltaTime);
        void render();
    
};