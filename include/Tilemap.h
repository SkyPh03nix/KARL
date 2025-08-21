#include <SFML/Graphics.hpp>
#include "Tile.h"

class Tilemap {
public:
    Tilemap(int width, int height, int tileSize);

    void load(const std::vector<std::vector<int>>& mapData, 
              const sf::Texture& tileset);

    void draw(sf::RenderWindow& window);

    bool isWalkable(int x, int y) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getTileSize() const { return tileSize; }

private:
    int width, height, tileSize;
    std::vector<std::vector<Tile>> tiles;
};