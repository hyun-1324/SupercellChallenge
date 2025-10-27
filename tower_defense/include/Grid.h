#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

enum class TileType {
    Empty,      // 타워 배치 가능
    Path,       // 적이 이동하는 경로
    Tower,      // 타워가 배치됨
    Start,      // 적 시작 지점
    End         // 적 도착 지점
};

class Grid {
public:
    Grid(int width, int height, int tileSize);

    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;

    sf::Vector2i worldToGrid(sf::Vector2f worldPos) const;
    sf::Vector2f gridToWorld(int x, int y) const;

    bool isValid(int x, int y) const;
    bool canPlaceTower(int x, int y) const;

    void draw(sf::RenderWindow& window) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getTileSize() const { return m_tileSize; }

private:
    int m_width;
    int m_height;
    int m_tileSize;
    std::vector<std::vector<TileType>> m_tiles;
};
