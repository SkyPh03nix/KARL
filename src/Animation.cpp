#include "Animation.h"
#include <iostream>

Animation::Animation(const std::vector<sf::IntRect>& frames, float frameTime, bool loop)
    : frames(frames), 
    frameTime(frameTime), 
    loop(loop), 
    currentFrame(0), 
    elapsedTime(0.f), 
    playing(false) {
}

void Animation::update(float deltaTime) {
    if (!playing || frames.empty()) return;
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.f;
        currentFrame++;
        if (currentFrame >= frames.size()) {
            if(loop) currentFrame=0;
            else {
                currentFrame = frames.size() - 1;
                playing=false;
            }
        }
    }
}
    
const sf::IntRect& Animation::getCurrentFrame() const {
    return frames[currentFrame];
}