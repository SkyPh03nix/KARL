#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>

class ResourceManager {
public:
    sf::Texture& getTexture(const std::string& name);
    void loadTexture(const std::string& name, const std::string& filename);
    bool hasTexture(const std::string& name) const;

private:
    std::map<std::string, sf::Texture> textures;
};