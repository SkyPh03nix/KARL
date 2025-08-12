#include "Utils.h"
#include <SFML/Graphics.hpp>

sf::Color getContrastingTextColor(const sf::Color& background) {
    //black text for lighter backgrounds, white text for darker background
    float luminance = 0.299f * background.r + 0.587f * background.g + 0.114f * background.b;
    return (luminance > 128) ? sf::Color::Black : sf::Color::White;
}