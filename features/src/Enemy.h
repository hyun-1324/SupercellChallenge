#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y);

    void update(float deltaTime, sf::Vector2f playerPos);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    bool isAlive() const { return m_alive; }
    void kill() { m_alive = false; }

private:
    sf::ConvexShape m_shape;  // 삼각형
    float m_speed;
    bool m_alive;
};
