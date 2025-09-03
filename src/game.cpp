#include "Game.h"
#include <iostream>
#include <cmath>
#include "Player.h"
#include "Button.h"
#include "Tree.h"
#include <fstream>
#include <random>
#include <memory>
#include <iostream>
#include "RecourceManager.h"

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

    //Button 1
    sf::Vector2f buttonPos(100, 100);
    auto button = std::make_unique<Button>(buttonSize,buttonPos,"Make Red" ); //TODO fitTextToButton() function
    button->setOnClick([&p = *player](){p.setColor(sf::Color::Red);});
    
    //Button 2
    sf::Vector2f button2Pos(100, 200);
    auto button2 = std::make_unique<Button>(buttonSize,button2Pos,"Make Green");
    button2->setOnClick([&p = *player](){p.setColor(sf::Color::Green);});

    //Button 3
    sf::Vector2f button3Pos(100, 300);
    auto button3 = std::make_unique<Button>(buttonSize,button3Pos,"Reset Color");
    button3->setOnClick([&p = *player](){p.setColor(sf::Color::White);});
    
    // !!! only do this at end of function (move destroys the local objects) !!!
    gameObjects.push_back(std::move(player));
    gameObjects.push_back(std::move(button)); 
    gameObjects.push_back(std::move(button2));
    gameObjects.push_back(std::move(button3)); 
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Game::initTextures() {
    resources.loadTexture("player_walk", "assets/used/Slime1_Walk_full.png");
    resources.loadTexture("player_idle", "assets/used/Slime1_Idle_full.png");
    resources.loadTexture("portal", "assets/used/Portals3.png");
    //resources.loadTexture("background", "assets/PixelArtPack/SceneOverview.png"); // for now better
    resources.loadTexture("background", "assets/used/grassbg.png");
    resources.loadTexture("tree", "assets/tree1.png");
    std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::initTrees() {

    //TODO save tree positions to file and load from it


    //std::ifstream in("gamesave.txt");
    //std::vector<sf::Vector2f> treePositions;

    //float x, y;
    //while (in >> x >> y) {
        //treePositions.emplace_back(x, y);
        //gameObjects.push_back(std::make_unique<Tree>(sf::Vector2f(x, y), resources.getTexture("assets/tree1.png")));
    //}
    //in.close();

    //std::mt19937 rng(std::random_device{}());

    //std::uniform_real_distribution<float> distX(
    //      camera.getCenter().x - window.getSize().x/2.f, 
    //      camera.getCenter().x + window.getSize().x/2.f
    //);
    //std::uniform_real_distribution<float> distY(
    //      camera.getCenter().y - window.getSize().y/2.f, 
    //      camera.getCenter().y + window.getSize().y/2.f
    //);

    //while (treePositions.size() < 100) {
    //    treePositions.emplace_back(distX(rng), distY(rng));
    //}

    //std::ofstream out("gamesave.txt");
    //for (const auto& pos : treePositions) {
    //    gameObjects.push_back(std::make_unique<Tree>(pos, resources.getTexture("assets/tree1.png")));
    //    out << pos.x << " " << pos.y << "\n";
    //}
}

Game::Game() : portals() {
    initWindow();
    initTextures();
    initBackground();
    initObjects();
    camera.setSize(window.getSize().x, window.getSize().y);
    
    initTrees();

    portals.setTexture(resources.getTexture("portal"));
    std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
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
    std::cout << __PRETTY_FUNCTION__ << std::endl; // debug
}

void Game::initBackground() {
    backgroundSprite.setTexture(resources.getTexture("background"));
    backgroundSprite.setScale(4, 4);
    std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
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
            portals.handleInput(mouseWorld, event.mouseButton.button);
        } else if(event.type == sf::Event::Closed) { //if we want to close the window by clicking the X button
            window.close(); 
        } 
    }
}

void Game::update(float deltaTime) {
    for (auto& obj : gameObjects) {
        obj->update(deltaTime, window);
    }
    portals.update(deltaTime, window);

    sf::Vector2f playerPos;
    for (auto& obj : gameObjects) {
        if (Player* player = dynamic_cast<Player*>(obj.get())) {
            portals.tryTeleport(*player);
            camera.setCenter(player->getPosition());
            playerPos = player->getPosition();
            break;
        }
    }

    sf::Vector2u winSize = window.getSize();
    float left   = playerPos.x - winSize.x / 2.f;
    float right  = playerPos.x + winSize.x / 2.f;
    float top    = playerPos.y - winSize.y / 2.f;
    float bottom = playerPos.y + winSize.y / 2.f;

    // delete trees outside of an extended view area
    float margin = 300.f;
    visibleTrees.erase(
        std::remove_if(visibleTrees.begin(), visibleTrees.end(),
            [left, right, top, bottom, margin](const std::unique_ptr<Tree>& tree) {
                sf::Vector2f pos = tree->getPosition();
                return (pos.x < left - margin || pos.x > right + margin ||
                        pos.y < top - margin  || pos.y > bottom + margin);
            }),
        visibleTrees.end()
    );

    //random values
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distX(left, right);
    std::uniform_real_distribution<float> distY(top, bottom);

    int currentCount     = static_cast<int>(visibleTrees.size());
    int targetTreeCount  = 3 + (rng() % 3); // 3, 4 or 5 trees

    while (currentCount < targetTreeCount) {
        sf::Vector2f newPos(distX(rng), distY(rng));
        visibleTrees.push_back(
            std::make_unique<Tree>(newPos, resources.getTexture("tree"))
        );
        currentCount++;
    }

    // debug 
    std::cout << "Bäume im Sichtfeld: " << visibleTrees.size() << std::endl;
    for (const auto& tree : visibleTrees) {
        sf::Vector2f pos = tree->getPosition();
        std::cout << "   Baum bei (" << pos.x << ", " << pos.y << ")\n";
    }
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
    }

    for (const auto& tree : visibleTrees) {
        tree->draw(window);
    }

    window.display();
}