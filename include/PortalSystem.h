#pragma once

#include "Portal.h"
#include "Player.h"

class PortalSystem {
    private:
        AnimationManager anims;
        Portal p1, p2;
        
        bool animationsInitialized = false;

        std::string currentAnimationP1;
        std::string currentAnimationP2;

        const float teleportCooldown = 0.f; //falls portale spaeter laengeren cooldown haben sollen //TODO entfernen wenn nicht
        float cooldownTimer = 0.f; //auch entfernen
        Portal* portalOnCooldown = nullptr;

    public:
        PortalSystem() : anims(), p1(anims), p2(anims) {}

        void setTexture(sf::Texture& texture);

        void handleInput(const sf::Vector2f& mouseWorldPos, sf::Mouse::Button button);
        
        void tryTeleport(Player& player);

        void draw(sf::RenderWindow& window) {p1.draw(window); p2.draw(window);}
        void update(float deltaTime, const sf::RenderWindow& window);
        
        bool isActive() {return p1.isActive() && p2.isActive();}
};