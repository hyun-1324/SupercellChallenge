#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f startPos, sf::Vector2f direction);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }
    bool isOffScreen(float screenWidth, float screenHeight) const;

private:
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    float m_speed;
};
