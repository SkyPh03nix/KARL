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
    auto playerObj = std::make_unique<Player>(
        resources.getTexture("player_walk"),
        resources.getTexture("player_idle"),
        sf::Vector2f(window.getSize().x/2.f-32.f, window.getSize().y/2.f-32.f),
        300.f
    );
    player = playerObj.get();
    gameObjects.push_back(std::move(playerObj));
}

void Game::initTextures() {
    resources.loadTexture("player_walk", "assets/used/Slime1_Walk_full.png");
    resources.loadTexture("player_idle", "assets/used/Slime1_Idle_full.png");
    resources.loadTexture("portal", "assets/used/Portals.png");
    resources.loadTexture("background", "assets/used/grassbg.png");
    resources.loadTexture("tree", "assets/used/tree.png");
    resources.loadTexture("trunk", "assets/used/stump.png");
    resources.loadTexture("wood", "assets/used/wood.png");
    resources.loadTexture("sapling", "assets/used/placeholder.png");
    resources.loadTexture("apple", "assets/used/placeholder2.png");
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
        auto tree = std::make_unique<Tree>(pos, resources.getTexture("tree"), resources.getTexture("trunk"), resources.getTexture("wood"), resources.getTexture("sapling"), resources.getTexture("apple"));
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
        auto tree = std::make_unique<Tree>(sf::Vector2f(x,y), resources.getTexture("tree"), resources.getTexture("trunk"), resources.getTexture("wood"), resources.getTexture("sapling"), resources.getTexture("apple"));
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
    uiView.setSize(window.getSize().x, window.getSize().y);
    uiView.setCenter(window.getSize().x/2.f, window.getSize().y/2.f);

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
            } else if (event.key.code == sf::Keyboard::Tab) {
                // Toggle inventory visibility
                for (auto& obj : gameObjects) {
                    if (auto* p = dynamic_cast<Player*>(obj.get())) {
                        p->getInventory().toggleVisibility();
                        break;
                    }
                }
            }
        } else if(event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, camera);
            sf::Vector2f uiMouse = window.mapPixelToCoords(mousePos, uiView);
            bool handled = false;

            //ui elements first
            if (event.mouseButton.button == sf::Mouse::Left) {
                // check inventory
                for (auto& obj : gameObjects) {
                    if (auto* p = dynamic_cast<Player*>(obj.get())) {
                        if (p->getInventory().isVisible() && p->getInventory().containsPoint(uiMouse)) {
                            p->getInventory().handleClick(uiMouse);
                            handled = true;
                            break;
                        }
                    }
                }
                // check buttons
                if (!handled) {
                    for (auto& obj : gameObjects) {
                        if (auto* btn = dynamic_cast<Button*>(obj.get())) {
                            sf::FloatRect btnBounds = btn->getBounds();
                            if (btnBounds.contains(uiMouse)) {
                                btn->click(); 
                                handled = true;
                                break;
                            }
                        }
                    }
                }
            }

            // world interactions if not handled by ui
            if(!handled) {
                // check for trees
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (auto& tree : visibleTrees) {
                        if (tree->getGlobalBounds().contains(mouseWorld)) {
                            tree->chop(worldItems);
                            handled = true;
                            break;
                        }
                    }
                }

                //place portal
                if(!handled){
                    portals.handleInput(mouseWorld, event.mouseButton.button);
                }   
            }                     
        } else if(event.type == sf::Event::Closed) {
            window.close(); 
        } 
    }
}

void Game::checkItemPickup() {
    for (auto& obj : gameObjects) {
        if (auto* player = dynamic_cast<Player*>(obj.get())) {
            for (auto it = worldItems.begin(); it != worldItems.end();) {
                if (player->getGlobalBounds().intersects((*it)->getGlobalBounds())) {
                    player->getInventory().addItem(*(*it));
                    it = worldItems.erase(it);
                } else {
                    ++it;
                }
            }
            break;
        }
    }
}

void Game::update(float deltaTime) {
    sf::Vector2f oldPlayerPos = player ? player->getPosition() : sf::Vector2f(0.f, 0.f);

    for (auto& obj : gameObjects) {obj->update(deltaTime, window);}
    for (auto& tree : visibleTrees) {tree->update(deltaTime, window);}
    for (auto& item : worldItems) {item->update(deltaTime, window);}

    portals.update(deltaTime, window);
    
    if (player) {
        checkItemPickup();
        
        portals.tryTeleport(*player);
        

        //add colision objects to resolve 
        std::vector<GameObject*> blockers;
        for (const auto& tree : visibleTrees) {
            if (!tree->isChopped()) {
                blockers.push_back(tree.get());
            }
        }
        resolveCollisions(oldPlayerPos, blockers);
        camera.setCenter(player->getPosition());
    }
}

void Game::resolveCollisions(const sf::Vector2f& oldPos, const std::vector<GameObject*>& blockers) {
    // Speichere die Zielposition nach Movement
    sf::Vector2f targetPos = player->getPosition();

    // 1. X-Richtung testen (Y bleibt alt)
    player->setPosition(sf::Vector2f(targetPos.x, oldPos.y));
    bool collisionX = false;
    for (const auto* blocker : blockers) {
        if (player->getGlobalBounds().intersects(blocker->getGlobalBounds())) {
            collisionX = true;
            break;
        }
    }
    float finalX = collisionX ? oldPos.x : targetPos.x;

    // 2. Y-Richtung testen (X ist jetzt finalX, Y ist die neue Zielposition!)
    player->setPosition(sf::Vector2f(finalX, targetPos.y));
    bool collisionY = false;
    for (const auto* blocker : blockers) {
        if (player->getGlobalBounds().intersects(blocker->getGlobalBounds())) {
            collisionY = true;
            break;
        }
    }
    float finalY = collisionY ? oldPos.y : targetPos.y;

    // Endgültige Position setzen
    player->setPosition(sf::Vector2f(finalX, finalY));
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
        if (Player* player = dynamic_cast<Player*>(obj.get())) {//debug
            auto pb = player->getGlobalBounds();                //debug
            sf::RectangleShape r({pb.width, pb.height});        //debug
            r.setPosition(pb.left, pb.top);                     //debug
            r.setFillColor(sf::Color(255, 0, 0, 100));          //debug
            window.draw(r);                                     //debug
        }
    }

    for (auto& item : worldItems) {
        item->draw(window);
        //draw bounds
        auto ib = item->getGlobalBounds();                      //debug
        sf::RectangleShape r({ib.width, ib.height});            //debug
        r.setPosition(ib.left, ib.top);                         //debug
        r.setFillColor(sf::Color(0, 0, 255, 100));              //debug
        window.draw(r);                                         //debug
    }

    for (const auto& tree : visibleTrees) {
        tree->draw(window);
        //draw bounds
        auto tb = tree->getGlobalBounds();                            //debug
        sf::RectangleShape r({tb.width, tb.height});            //debug
        r.setPosition(tb.left, tb.top);                         //debug
        r.setFillColor(sf::Color(0, 255, 0, 100));              //debug
        window.draw(r);
    }

    // draw UI elements like inventory
    window.setView(uiView);

    for (auto& obj : gameObjects) {
        if (auto* btn = dynamic_cast<Button*>(obj.get())) {
            btn->draw(window);
        }
        if (auto* p = dynamic_cast<Player*>(obj.get())) {
            p->getInventory().draw(window);
        }
    }

    window.display();
}