#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Bullet;

class Player {
public:
    Player();

    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void updateAimLine(sf::Vector2f mousePos);

    bool canShoot() const;
    void shoot(sf::Vector2f mousePos);
    void resetShootCooldown();

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void pushBack(sf::Vector2f direction, float amount, float deltaTime);

private:
    sf::CircleShape m_shape;
    sf::RectangleShape m_aimLine;
    float m_speed;
    float m_shootCooldown;
    float m_shootInterval;
};
