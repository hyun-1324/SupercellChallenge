#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(std::vector<sf::Vector2i> path, int tileSize);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void takeDamage(int damage);

    bool isDead() const { return m_hp <= 0; }
    bool hasReachedEnd() const { return m_reachedEnd; }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

private:
    sf::CircleShape m_shape;
    std::vector<sf::Vector2f> m_path;  // World coordinates
    int m_currentWaypoint;
    float m_speed;
    int m_hp;
    int m_maxHp;
    bool m_reachedEnd;

    void updateColor();
};
