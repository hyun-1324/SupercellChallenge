#pragma once

#include <SFML/Graphics.hpp>

class Enemy;

class Projectile {
public:
    Projectile(sf::Vector2f position, Enemy* target, float damage);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    bool shouldRemove() const { return m_shouldRemove; }

private:
    sf::CircleShape m_shape;
    Enemy* m_target;
    float m_speed;
    float m_damage;
    bool m_shouldRemove;
};
