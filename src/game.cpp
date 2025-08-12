#include "Game.h"
#include <iostream>
#include "Player.h"
#include "Button.h"
#include <memory>
#include <iostream>
#include "RecourceManager.h"

void Game::initObjects() {
    //Karl
    auto player = std::make_unique<Player>(
        resources.getTexture("player_walk"),
        resources.getTexture("player_idle"),
        sf::Vector2f(window.getSize().x/2.f-32.f, window.getSize().y/2.f-32.f),
        250.f
    );
    //Button
    sf::Vector2f buttonSize(100.f,50.f);
    sf::Vector2f buttonPos(100, 100);

    auto button = std::make_unique<Button>(
        buttonSize,
        buttonPos,
        "Make Red" //TODO fitTextToButton() function
    );
    button->setOnClick([&p = *player](){
        p.setColor(sf::Color::Red);
    });
    
    //Button 2
    sf::Vector2f button2Pos(100, 200);
    auto button2 = std::make_unique<Button>(
        buttonSize,
        button2Pos,
        "Make Green"
    );
    button2->setOnClick([&p = *player](){
        p.setColor(sf::Color::Green);
    });
    
    // !!! only do this at end of function (move destroys the local objects) !!!
    gameObjects.push_back(std::move(player));
    gameObjects.push_back(std::move(button)); 
    gameObjects.push_back(std::move(button2));   
}

void Game::initTextures() {
    resources.loadTexture("player_walk", "assets/slime/PNG/Slime1/Walk/Slime1_Walk_full.png");
    resources.loadTexture("player_idle", "assets/slime/PNG/Slime1/Idle/Slime1_Idle_full.png");
    resources.loadTexture("portal", "assets/myDesigns/Portals3.png");
    resources.loadTexture("background", "assets/background2.png");
}

Game::Game() : portals() {
    initWindow();
    initTextures();
    initBackground();
    initObjects();

    portals.setTexture(resources.getTexture("portal"));
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
}

void Game::initBackground() {
        backgroundSprite.setTexture(resources.getTexture("background"));

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = resources.getTexture("background").getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
        if(event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            portals.handleInput(mouseWorld, event.mouseButton.button);
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
            break;
        }
    }
}

void Game::render() {
    window.clear();
    window.draw(backgroundSprite);

    portals.draw(window);

    for (auto& obj : gameObjects) {
        obj->draw(window);
    }
    window.display();
}