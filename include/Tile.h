#include <SFML/Graphics.hpp>

class Tile {
    public:
        Tile(const sf::Texture& texture, sf::Vector2f pos, bool walkable) : walkable(walkable) {
            sprite.setTexture(texture);
            sprite.setPosition(pos);
        }

        void draw(sf::RenderWindow& window) const {
            window.draw(sprite);
        }

        bool isWalkable() const {return walkable;}
        sf::FloatRect getBounds() const {return sprite.getGlobalBounds();}

    private:
        sf::Sprite sprite;
        bool walkable;
};