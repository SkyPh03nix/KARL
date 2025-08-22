#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height) {
        // Tileset laden
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // VertexArray vorbereiten
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // Für jedes Tile
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                // Tile-Nummer aus den Daten
                int tileNumber = tiles[x + y * width];

                // Position im Tileset (Spalte, Zeile)
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // Zeiger auf die 4 Vertices (Quad)
                sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

                // Position (auf dem Bildschirm)
                quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
                quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

                // Textur-Koordinaten (welcher Ausschnitt vom Tileset)
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }
        return true;
    }

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Transformation anwenden
        states.transform *= getTransform();

        // Tileset als Textur binden
        states.texture = &m_tileset;

        // Alle Vertices zeichnen
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};
