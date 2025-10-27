#include "../include/Grid.h"
#include "../include/Item.h"

Grid::Grid(int width, int height, int cellSize)
    : m_width(width)
    , m_height(height)
    , m_cellSize(cellSize)
{
    // Initialize grid to nullptr
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            m_cells[y][x] = nullptr;
        }
    }
}

void Grid::draw(sf::RenderWindow& window) const {
    // Draw grid lines
    for (int y = 0; y <= m_height; y++) {
        sf::RectangleShape line({static_cast<float>(m_width * m_cellSize), 2});
        line.setPosition({0, static_cast<float>(y * m_cellSize)});
        line.setFillColor(sf::Color(100, 100, 100));
        window.draw(line);
    }

    for (int x = 0; x <= m_width; x++) {
        sf::RectangleShape line({2, static_cast<float>(m_height * m_cellSize)});
        line.setPosition({static_cast<float>(x * m_cellSize), 0});
        line.setFillColor(sf::Color(100, 100, 100));
        window.draw(line);
    }

    // Draw cell backgrounds
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            sf::RectangleShape cell({static_cast<float>(m_cellSize - 4),
                                     static_cast<float>(m_cellSize - 4)});
            cell.setPosition({static_cast<float>(x * m_cellSize + 2),
                             static_cast<float>(y * m_cellSize + 2)});
            cell.setFillColor(sf::Color(40, 40, 40));
            window.draw(cell);
        }
    }
}

bool Grid::isValidPosition(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

bool Grid::isEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return m_cells[y][x] == nullptr;
}

void Grid::setItem(int x, int y, Item* item) {
    if (isValidPosition(x, y)) {
        m_cells[y][x] = item;
        if (item) {
            item->setGridPosition(x, y);
        }
    }
}

Item* Grid::getItem(int x, int y) const {
    if (!isValidPosition(x, y)) return nullptr;
    return m_cells[y][x];
}

void Grid::removeItem(int x, int y) {
    if (isValidPosition(x, y)) {
        m_cells[y][x] = nullptr;
    }
}

sf::Vector2f Grid::gridToWorld(int x, int y) const {
    return {x * m_cellSize + m_cellSize / 2.0f,
            y * m_cellSize + m_cellSize / 2.0f};
}

sf::Vector2i Grid::worldToGrid(sf::Vector2f worldPos) const {
    int x = static_cast<int>(worldPos.x / m_cellSize);
    int y = static_cast<int>(worldPos.y / m_cellSize);
    return {x, y};
}
