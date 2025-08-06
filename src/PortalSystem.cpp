 #include "PortalSystem.h"

void PortalSystem::handleInput(const sf::Vector2f& mouseWorldPos, sf::Mouse::Button button) {
    if (button == sf::Mouse::Left) {
        p1.place(mouseWorldPos, sf::Color(255, 128, 0));  // Orange
    } else if (button == sf::Mouse::Right) {
        p2.place(mouseWorldPos, sf::Color::Blue);
    }
}

void PortalSystem::update(float deltaTime) {
    if (cooldownTimer > 0.f) {
        cooldownTimer -= deltaTime;
        if (cooldownTimer < 0.f)
            cooldownTimer = 0.f;
    }
}

void PortalSystem::tryTeleport(Player& player) {
    if (cooldownTimer > 0.f)
        return;

    bool onP1 = player.getGlobalBounds().intersects(p1.getBounds());
    bool onP2 = player.getGlobalBounds().intersects(p2.getBounds());

    if (onP1 && portalOnCooldown != &p1) {
        if(!p2.isActive()) return;
        player.setPosition(p2.getPosition());
        portalOnCooldown = &p2;
        cooldownTimer = teleportCooldown;
    } else if (onP2 && portalOnCooldown != &p2) {
        if(!p1.isActive()) return;
        player.setPosition(p1.getPosition());
        portalOnCooldown = &p1;
        cooldownTimer = teleportCooldown;
    } else if (!onP1 && !onP2) {
        portalOnCooldown = nullptr;
    }
}