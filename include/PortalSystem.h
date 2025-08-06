#pragma once

#include "Portal.h"
#include "Player.h"

class PortalSystem {
    private:
        Portal p1;
        Portal p2;
        const float teleportCooldown = 0.f; //falls portale spaeter laengeren cooldown haben sollen //TODO entfernen wenn nicht
        float cooldownTimer = 0.f; //auch entfernen
        Portal* portalOnCooldown = nullptr;
    public:
        void handleInput(const sf::Vector2f& mouseWorldPos, sf::Mouse::Button button);
        bool isActive() {return p1.isActive() && p2.isActive();}
        void draw(sf::RenderWindow& window) {p1.draw(window); p2.draw(window);}
        void update(float deltaTime);
        void tryTeleport(Player& player);
};