#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Item;

class Grid {
public:
    Grid(int width, int height, int cellSize);

    void draw(sf::RenderWindow& window) const;

    bool isValidPosition(int x, int y) const;
    bool isEmpty(int x, int y) const;

    void setItem(int x, int y, Item* item);
    Item* getItem(int x, int y) const;
    void removeItem(int x, int y);

    sf::Vector2f gridToWorld(int x, int y) const;
    sf::Vector2i worldToGrid(sf::Vector2f worldPos) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getCellSize() const { return m_cellSize; }

private:
    int m_width;
    int m_height;
    int m_cellSize;
    Item* m_cells[8][8];  // Grid storage
};
