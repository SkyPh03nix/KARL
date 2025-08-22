#include <SFML/Graphics.hpp>


enum StatusEffect {Default = 0, Slow, Damage};
class Tile {
    public:
        Tile(const sf::Texture& texture, sf::Vector2f pos, bool walkable, StatusEffect effect = StatusEffect::Default) 
        : walkable(walkable), effect(effect) {
            sprite.setTexture(texture);
            sprite.setPosition(pos);
        }

        void draw(sf::RenderWindow& window) const {
            window.draw(sprite);
        }

        bool isWalkable() const {return walkable;}
        sf::FloatRect getBounds() const {return sprite.getGlobalBounds();}

        StatusEffect getEffect() const { return effect; }

    private:
        sf::Sprite sprite;
        bool walkable;        
        StatusEffect effect;
};