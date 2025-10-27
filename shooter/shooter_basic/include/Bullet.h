#pragma once

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Vector2f position, sf::Vector2f velocity);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    bool isOffScreen() const;
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
};
