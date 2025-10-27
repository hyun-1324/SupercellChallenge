#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(sf::Vector2f position, int wave);

    void update(float deltaTime, sf::Vector2f playerPos);
    void draw(sf::RenderWindow& window) const;
    void takeDamage(int damage);

    bool isDead() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

private:
    void updateColor();

    sf::CircleShape m_shape;
    int m_hp;
    int m_maxHp;
    float m_speed;
    int m_wave;
};
