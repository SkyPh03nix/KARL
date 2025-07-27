#include "Entity.h"
#include "Player.h"
#include "Button.h"

int main() {
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(mode, "test", sf::Style::None);
    sf::Clock clock;
    
    sf::Vector2u windowSize = window.getSize();
    float speed = 1500.f;

    //karl
    Player karl({50.f,50.f}, {float(windowSize.x/2-25),float(windowSize.y/2-25)}, speed, "Karl");
    Button colorChangeButton({20.f,20.f}, {float(windowSize.x/2-10),float(windowSize.y/2-10)});

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // closing the window
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) 
                    window.close();
            }
        }
        
    karl.update(deltaTime, windowSize);
    colorChangeButton.update(karl);

    window.clear();

    colorChangeButton.draw(window);
    karl.draw(window);

    window.display();
    }
    return 0;
}