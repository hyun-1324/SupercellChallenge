#pragma once

#include <SFML/Graphics.hpp>

class Item {
public:
    Item(int level, sf::Vector2f position);

    void draw(sf::RenderWindow& window) const;
    void setPosition(sf::Vector2f position);
    void setGridPosition(int x, int y);

    int getLevel() const { return m_level; }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::Vector2i getGridPosition() const { return m_gridPos; }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

    void setDragging(bool dragging) { m_isDragging = dragging; }
    bool isDragging() const { return m_isDragging; }

private:
    int m_level;  // 1-7
    sf::RectangleShape m_shape;
    sf::Vector2i m_gridPos;  // Grid coordinates
    bool m_isDragging;

    void updateColor();
};
