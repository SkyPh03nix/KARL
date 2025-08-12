#include "RecourceManager.h"

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }

    sf::Texture tex;
    if (!tex.loadFromFile(name)) {
        throw std::runtime_error("Could not load texture: " + name);
    }
    textures[name] = std::move(tex);
    return textures[name];
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        throw std::runtime_error("ResourceManager: Fehler beim Laden von " + filename);
    }
    texture.setSmooth(false); 
    textures[name] = std::move(texture);
}

bool ResourceManager::hasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}