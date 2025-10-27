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

    // HP system
    void takeDamage(int damage);
    int getHP() const;
    bool isDead() const;
    bool isInvincible() const;
    void reset();

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void pushBack(sf::Vector2f direction, float amount, float deltaTime);

private:
    sf::CircleShape m_shape;
    sf::RectangleShape m_aimLine;
    float m_speed;
    float m_shootCooldown;
    float m_shootInterval;

    // HP system
    int m_hp;
    int m_maxHP;
    float m_invincibilityTimer;
};
