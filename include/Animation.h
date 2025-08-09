#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
    private:
        std::vector<sf::IntRect> frames;

        float frameTime = 0.1f;
        bool loop=true;

        size_t currentFrame = 0;
        float elapsedTime = 0.f;
        bool playing = false;
        
    public:
        Animation() = default;
        Animation(const std::vector<sf::IntRect>& frames, float frameTime, bool loop=true);

        void play() { playing = true; currentFrame = 0; elapsedTime = 0.f;}
        void pause() { playing = false; }
        void stop() { playing = false; currentFrame = 0; elapsedTime = 0.f; }

        void update(float deltaTime);

        const sf::IntRect& getCurrentFrame() const;
};