#include "Entity.h"

int main() {
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(mode, "test", sf::Style::None);
    sf::Clock clock;
    
    sf::Vector2u size = window.getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;
    float speed = 1500.f;

    //karl
    sf::RectangleShape karl(sf::Vector2f(50.f,50.f));
    karl.setFillColor(sf::Color::Red);
    karl.setPosition(width/2-25, height/2-25);

    //color change button
    sf::RectangleShape colorChangeButton(sf::Vector2f(50.f,50.f));
    colorChangeButton.setOutlineColor(sf::Color::Green);
    colorChangeButton.setOutlineThickness(5.f);
    colorChangeButton.setPosition(width/2-25, height/2-25);

    std::vector<sf::RectangleShape> rectangles;

    while (window.isOpen()) {

        float deltaTime = clock.restart().asSeconds();

        // closing the window
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) 
                    window.close();
                if(event.key.code == sf::Keyboard::P) 
                    window.clear();
            }
        }
        // movement
        sf::Vector2f movement(0.f,0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * deltaTime;
        karl.move(movement);

        // checking for wall collision and negating the movement 
        sf::FloatRect bounds = karl.getGlobalBounds();
        if (bounds.left < 0) {karl.setPosition(0,bounds.top);}
        if (bounds.left + bounds.width > width) {karl.setPosition(width-bounds.width,bounds.top);}
        if (bounds.top < 0) {karl.setPosition(bounds.left, 0);}
        if (bounds.top + bounds.height > height) {karl.setPosition(bounds.left, height - bounds.height);}

        rectangles.push_back(karl);
        rectangles.push_back(colorChangeButton);

        window.clear();
        
        window.draw(karl);
        window.draw(colorChangeButton);
        window.display();
    }
    return 0;
}