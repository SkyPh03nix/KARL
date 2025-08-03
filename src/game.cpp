#include "Game.h"
#include <iostream>


void Game::initObjects() {
    //sf::Vector2u windowSize = window.getSize(); // for window size based placement of objects

    //Karl
    auto player = std::make_unique<Player>(
        sf::Vector2f(50.f,50.f), 
        sf::Vector2f(window.getSize().x/2.f-25.f, window.getSize().y/2.f-25.f),
        900.f,
        "Karl"
    );
    
    //Button
    sf::Vector2f buttonSize(100.f,50.f);
    sf::Vector2f buttonPos(100, 100);
    auto button = std::make_unique<Button>(
        buttonSize,
        buttonPos,
        "Make Red"
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


Game::Game() {
    initWindow();
    initObjects();
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
    sf::VideoMode mode = sf::VideoMode::getDesktopMode(); //TODO research desktop and fullscreen modes
    window.create(mode, "Karl", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
}


void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
    }
}

void Game::update(float deltaTime) {
    for (auto& obj : gameObjects) {
        obj->update(deltaTime, window);
    }
}

void Game::render() {
    window.clear();

    for (auto& obj : gameObjects) {
        obj->draw(window);
    }
    window.display();
}