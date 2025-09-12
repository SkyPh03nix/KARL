 #include "PortalSystem.h"
 #include <iostream>

void PortalSystem::handleInput(const sf::Vector2f& mouseWorldPos, sf::Mouse::Button button) {
    if (button == sf::Mouse::Left) {
        p1.place(mouseWorldPos, sf::Color(255, 128, 0), "orange");
    } else if (button == sf::Mouse::Right) {
        p2.place(mouseWorldPos, sf::Color::Blue, "blue");
    }
}

void PortalSystem::update(float deltaTime, const sf::RenderWindow& window) {
    anims.update(deltaTime);
    p1.update(deltaTime, window);
    p2.update(deltaTime, window);
}

void PortalSystem::setTexture(sf::Texture& tex) {
    if (!animationsInitialized) {
        std::vector<sf::IntRect> orangeFrames;
        for (int i = 0; i < 4; i++) {
            orangeFrames.emplace_back(i * 64, 0, 64, 64);
        }
        std::vector<sf::IntRect> blueFrames;
        for (int i = 0; i < 4; i++) {
            blueFrames.emplace_back(i * 64, 64, 64, 64);
        }
        std::vector<sf::IntRect> greyFrames;
        for (int i = 0; i < 4; i++) {
            greyFrames.emplace_back(i * 64, 128, 64, 64);
        }

        anims.addAnimation("orange", tex, orangeFrames, 0.3f, true);
        anims.addAnimation("blue", tex, blueFrames, 0.3f, true);
        anims.addAnimation("grey", tex, greyFrames, 0.3f, true);

        animationsInitialized = true;
        //std::cout << "Animations initialized for PortalSystem\n";
    }

    p1.setTexture(tex);
    p2.setTexture(tex);
}

void PortalSystem::tryTeleport(Player& player) {
    if (cooldownTimer > 0.f)
        return;

    bool onP1 = player.getGlobalBounds().intersects(p1.getGlobalBounds());
    bool onP2 = player.getGlobalBounds().intersects(p2.getGlobalBounds());

    if (onP1 && portalOnCooldown != &p1) {
        if(!p2.isActive()) return;
        sf::Vector2f portalCenter = p2.getPosition();
        player.setPosition(portalCenter);
        portalOnCooldown = &p2;
        cooldownTimer = teleportCooldown;
    } else if (onP2 && portalOnCooldown != &p2) {
        if(!p1.isActive()) return;
        sf::Vector2f portalCenter = p1.getPosition();
        player.setPosition(portalCenter);
        portalOnCooldown = &p1;
        cooldownTimer = teleportCooldown;
    } else if (!onP1 && !onP2) {
        portalOnCooldown = nullptr;
    }
}