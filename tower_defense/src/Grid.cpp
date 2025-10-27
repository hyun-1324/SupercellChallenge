#include "../include/Grid.h"

Grid::Grid(int width, int height, int tileSize)
    : m_width(width)
    , m_height(height)
    , m_tileSize(tileSize)
{
    m_tiles.resize(m_height, std::vector<TileType>(m_width, TileType::Empty));
}

void Grid::setTile(int x, int y, TileType type) {
    if (isValid(x, y)) {
        m_tiles[y][x] = type;
    }
}

TileType Grid::getTile(int x, int y) const {
    if (isValid(x, y)) {
        return m_tiles[y][x];
    }
    return TileType::Empty;
}

sf::Vector2i Grid::worldToGrid(sf::Vector2f worldPos) const {
    return sf::Vector2i(
        static_cast<int>(worldPos.x / m_tileSize),
        static_cast<int>(worldPos.y / m_tileSize)
    );
}

sf::Vector2f Grid::gridToWorld(int x, int y) const {
    return sf::Vector2f(
        x * m_tileSize + m_tileSize / 2.0f,
        y * m_tileSize + m_tileSize / 2.0f
    );
}

bool Grid::isValid(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

bool Grid::canPlaceTower(int x, int y) const {
    return isValid(x, y) && m_tiles[y][x] == TileType::Empty;
}

void Grid::draw(sf::RenderWindow& window) const {
    sf::RectangleShape tile(sf::Vector2f(m_tileSize - 2.0f, m_tileSize - 2.0f));

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            tile.setPosition({static_cast<float>(x * m_tileSize + 1), static_cast<float>(y * m_tileSize + 1)});

            switch (m_tiles[y][x]) {
                case TileType::Empty:
                    tile.setFillColor(sf::Color(50, 50, 50));
                    break;
                case TileType::Path:
                    tile.setFillColor(sf::Color(139, 69, 19));  // Brown
                    break;
                case TileType::Tower:
                    tile.setFillColor(sf::Color(100, 100, 100));
                    break;
                case TileType::Start:
                    tile.setFillColor(sf::Color::Red);
                    break;
                case TileType::End:
                    tile.setFillColor(sf::Color::Blue);
                    break;
            }

            window.draw(tile);
        }
    }
}
