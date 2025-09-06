#include "Game.h"
#include "Player.h"
#include "Button.h"
#include "Tree.h"
#include "RecourceManager.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <memory>

void Game::initObjects() {
    //Player
    auto player = std::make_unique<Player>(
        resources.getTexture("player_walk"),
        resources.getTexture("player_idle"),
        sf::Vector2f(window.getSize().x/2.f-32.f, window.getSize().y/2.f-32.f),
        300.f
    );

    //Buttons
    sf::Vector2f buttonSize(100.f,50.f);

    auto button = std::make_unique<Button>(buttonSize,sf::Vector2f(100,100),"Make Red" ); //TODO fitTextToButton() function
    button->setOnClick([&p = *player](){p.setColor(sf::Color::Red);});
    
    auto button2 = std::make_unique<Button>(buttonSize,sf::Vector2f(100,200),"Make Green");
    button2->setOnClick([&p = *player](){p.setColor(sf::Color::Green);});

    auto button3 = std::make_unique<Button>(buttonSize,sf::Vector2f(100,300),"Reset Color");
    button3->setOnClick([&p = *player](){p.setColor(sf::Color::White);});
    
    // !!! only do this at end of function (move destroys the local objects) !!!
    gameObjects.push_back(std::move(player));
    gameObjects.push_back(std::move(button)); 
    gameObjects.push_back(std::move(button2));
    gameObjects.push_back(std::move(button3)); 
    //std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::initTextures() {
    resources.loadTexture("player_walk", "assets/used/Slime1_Walk_full.png");
    resources.loadTexture("player_idle", "assets/used/Slime1_Idle_full.png");
    resources.loadTexture("portal", "assets/used/Portals.png");
    resources.loadTexture("background", "assets/used/grassbg.png");
    resources.loadTexture("tree", "assets/used/tree.png");
    resources.loadTexture("trunk", "assets/used/stump.png");
}

void Game::initTrees() {
    const std::string filename = "gamesave.txt";
    std::ifstream in(filename);
    if (in.is_open()) {
        in.close();
        loadTreesFromFile(filename);
    } else {
        std::cout << "No save file found, generating new trees." << std::endl;
        static std::mt19937 rng(std::random_device{}());

        float areaSize = 2000.f;
        int treeCount = 50;
        float minDist = 100.f; // minimum distance between trees and player start

        std::uniform_real_distribution<float> dist(0, areaSize);

        //get player start position
        sf::Vector2f playerPos(0.f, 0.f);
        for (auto& obj : gameObjects) {
            if (auto* p = dynamic_cast<Player*>(obj.get())) {
                playerPos = p->getPosition(); break;
            }
        }

        std::vector<sf::Vector2f> positions;
        int tries = 0;
        int maxTries = treeCount * 10; // prevent infinite loop

        while ((int)positions.size() < treeCount && tries < maxTries) {
        sf::Vector2f pos(dist(rng), dist(rng));
        bool tooClose = false;

        // Abstand zu Spieler prüfen
        float dxP = pos.x - playerPos.x;
        float dyP = pos.y - playerPos.y;
        if (std::sqrt(dxP*dxP + dyP*dyP) < minDist) {
            tooClose = true;
        }

        // Abstand zu anderen Bäumen prüfen
        for (const auto& other : positions) {
            float dx = pos.x - other.x;
            float dy = pos.y - other.y;
            if (std::sqrt(dx*dx + dy*dy) < minDist) {
                tooClose = true;
                break;
            }
        }

        if (!tooClose) {
            positions.push_back(pos);
        }

        ++tries;
    }
    
    //sort trees by y for better rendering order
    std::sort(positions.begin(), positions.end(), [](const sf::Vector2f& a, const sf::Vector2f& b) {
        return (a.y < b.y) || (a.y == b.y && a.x < b.x);
    });

    for (const auto& pos : positions) {
        auto tree = std::make_unique<Tree>(pos, resources.getTexture("tree"), resources.getTexture("trunk"));
        tree->setScale(3.f, 3.f);
        visibleTrees.push_back(std::move(tree));
    }

    saveTreesToFile(filename);
    }
}


void Game::loadTreesFromFile(const std::string& filename) {
    visibleTrees.clear();
    std::ifstream in(filename);
    if (!in.is_open()) { 
        std::cerr << "Could not open " << filename << " for reading!" << std::endl;
        return;
    }

    float x,y;
    while (in >> x >> y) {
        auto tree = std::make_unique<Tree>(sf::Vector2f(x,y), resources.getTexture("tree"), resources.getTexture("trunk"));
        tree->setScale(3.f, 3.f);
        visibleTrees.push_back(std::move(tree));
    }
    in.close();
    std::cout << "Loaded " << visibleTrees.size() << " trees from " << filename << std::endl;
}

void Game::saveTreesToFile(const std::string& filename) {
    std::ofstream out(filename);
    if(!out.is_open()) {
        std::cerr << "Could not open " << filename << " for writing!" << std::endl;
        return;
    }

    std::vector<sf::Vector2f> treePositions;
    for (const auto& tree : visibleTrees) {
        treePositions.push_back(tree->getPosition());
    }
    std::sort(treePositions.begin(), treePositions.end(), [](const sf::Vector2f& a, const sf::Vector2f& b) {
        return (a.y < b.y) || (a.y == b.y && a.x < b.x);
    });

    for (const auto& pos : treePositions) {
        out << pos.x << " " << pos.y << "\n";
    }
    out.close();
    std::cout << "Saved " << visibleTrees.size() << " trees to " << filename << std::endl;
}

Game::Game() : portals() {
    initWindow();
    initTextures();
    initBackground();
    initObjects();
    camera.setSize(window.getSize().x, window.getSize().y);
    initTrees();
    portals.setTexture(resources.getTexture("portal"));
    //std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::initWindow() {
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.create(mode, "Karl", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    //std::cout << __PRETTY_FUNCTION__ << std::endl; // debug
}

void Game::initBackground() {
    backgroundSprite.setTexture(resources.getTexture("background"));
    backgroundSprite.setScale(4, 4);
    //std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        } else if(event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            bool handled = false;

            //check for Button click:
            for (auto& obj : gameObjects) {
                if (auto* btn = dynamic_cast<Button*>(obj.get())) {
                    if (btn->getBounds().contains(mouseWorld)) {
                        btn->click();  // Button wurde gedrückt
                        handled = true;
                        break;
                    }
                }
            }

            // check for trees
            if (!handled && event.mouseButton.button == sf::Mouse::Left) {
                for (auto& tree : visibleTrees) {
                    if (tree->getBounds().contains(mouseWorld)) {
                        tree->chop(); // Baum fällen
                        handled = true;
                        break;
                    }
                }
            }

            //place portal
            if(!handled){
                portals.handleInput(mouseWorld, event.mouseButton.button);
            }            
        } else if(event.type == sf::Event::Closed) {
            window.close(); 
        } 
    }
}

void Game::update(float deltaTime) {
    Player* playerPtr = nullptr;
    for (auto& obj : gameObjects) {
        if (auto* p = dynamic_cast<Player*>(obj.get())) {
            playerPtr = p;
            break;
        }
    }

    sf::Vector2f oldPlayerPos = playerPtr ? playerPtr->getPosition() : sf::Vector2f(0.f, 0.f);

    for (auto& obj : gameObjects) {
        obj->update(deltaTime, window);
    }
    for (auto& tree : visibleTrees) {
        tree->update(deltaTime, window);
    }
    portals.update(deltaTime, window);
    
    
    if (playerPtr) {
        // check for portal collisions
        portals.tryTeleport(*playerPtr);
        camera.setCenter(playerPtr->getPosition());

        // check for tree collisions
        for (const auto& tree : visibleTrees) {
            if (playerPtr->getGlobalBounds().intersects(tree->getBounds())) {
                playerPtr->setPosition(oldPlayerPos); // Revert to old position on collision
                camera.setCenter(oldPlayerPos);
                break;
            }
        }
    }

    /*
    // debug 
    std::cout << "Bäume im Sichtfeld: " << visibleTrees.size() << std::endl;
    for (const auto& tree : visibleTrees) {
        sf::Vector2f pos = tree->getPosition();
        std::cout << "   Baum bei (" << pos.x << ", " << pos.y << ")\n";
    }
    */
}

void Game::render() {
    window.clear();
    window.setView(camera);

    //draw background repeatedly untill view is filled
    sf::Vector2f camPos = camera.getCenter();
    sf::Vector2u texSize = resources.getTexture("background").getSize();
    sf::Vector2u winSize = window.getSize();

    // round to nearest tile size below
    float startX = std::floor((camPos.x - winSize.x / 2.f) / texSize.x) * texSize.x;
    float startY = std::floor((camPos.y - winSize.y / 2.f) / texSize.y) * texSize.y;

    // number of tiles needed to fill the view
    int tilesX = winSize.x / texSize.x + 2;
    int tilesY = winSize.y / texSize.y + 2;

    sf::Sprite bg(resources.getTexture("background"));

    //draw all background tiles
    for (int i = 0; i < tilesX; ++i) {
        for (int j = 0; j < tilesY; ++j) {
            bg.setPosition(startX + i * texSize.x, startY + j * texSize.y);
            window.draw(bg); 
        }
    }

    portals.draw(window);

    for (auto& obj : gameObjects) {
        obj->draw(window);
        //draw bounds
        if (Player* player = dynamic_cast<Player*>(obj.get())) { //debug
        auto pb = player->getGlobalBounds(); //debug
        sf::RectangleShape r({pb.width, pb.height}); //debug
        r.setPosition(pb.left, pb.top); //debug
        r.setFillColor(sf::Color(255, 0, 0, 100)); //debug
        window.draw(r); //debug
    }
    }

    for (const auto& tree : visibleTrees) {
        tree->draw(window);
        //draw bounds
        auto tb = tree->getBounds(); //debug
        sf::RectangleShape r({tb.width, tb.height}); //debug
        r.setPosition(tb.left, tb.top); //debug
        r.setFillColor(sf::Color(0, 255, 0, 100)); //debug
        window.draw(r);
    }

    window.display();
}