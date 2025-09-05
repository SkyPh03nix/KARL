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
    //std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::initTrees() {

    //TODO save tree positions to file and load from it


    std::ifstream in("gamesave.txt");
    if (in.is_open()) {
        float x,y;
        while (in >> x >> y) {
            auto tree = std::make_unique<Tree>(sf::Vector2f(x,y), resources.getTexture("tree"));
            tree->setScale(3.f, 3.f);
            visibleTrees.push_back(std::move(tree));
        }
        in.close();
        std::cout << "Loaded " << visibleTrees.size() << " trees from gamesave.txt" << std::endl;
    } else {
        std::cout << "Could not open gamesave.txt for reading. Starting with no trees." << std::endl;
        static std::mt19937 rng(std::random_device{}());

        float areaSize = 2000.f; // area in which trees can spawn
        int TreeCount = 50; // number of trees to spawn

        std::uniform_real_distribution<float> dist(-areaSize/2.f, areaSize/2.f);
        for (int i = 0; i < TreeCount; ++i) {
            sf::Vector2f pos(dist(rng), dist(rng));
            auto tree = std::make_unique<Tree>(pos, resources.getTexture("tree"));
            tree->setScale(3.f, 3.f);
            visibleTrees.push_back(std::move(tree));
        }

        std::ofstream out("gamesave.txt");
        for (const auto& tree : visibleTrees) {
            sf::Vector2f pos = tree->getPosition();
            out << pos.x << " " << pos.y << "\n";
        }
        out.close();
    }
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
            portals.handleInput(mouseWorld, event.mouseButton.button);
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