#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Projectile.h"

class Enemy;

class Tower {
public:
    Tower(sf::Vector2f position);

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

    std::vector<std::unique_ptr<Projectile>>& getProjectiles() { return m_projectiles; }

private:
    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
    void shoot(Enemy* target);

    sf::RectangleShape m_shape;
    sf::CircleShape m_rangeCircle;  // Visual range indicator
    float m_range;
    float m_damage;
    float m_fireRate;
    float m_cooldown;

    std::vector<std::unique_ptr<Projectile>> m_projectiles;
};
