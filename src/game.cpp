#include "Game.h"
#include <iostream>
#include <cmath>
#include "Player.h"
#include "Button.h"
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
    resources.loadTexture("background", "assets/grassbg.png");
    std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

Game::Game() : portals() {
    initWindow();
    initTextures();
    initBackground();
    initObjects();

    camera.setSize(window.getSize().x, window.getSize().y);

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

    for (auto& obj : gameObjects) {
        Player* player = dynamic_cast<Player*>(obj.get());
        if (player) {
            portals.tryTeleport(*player);
            camera.setCenter(player->getPosition());
            break;
        }
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
    window.display();
}