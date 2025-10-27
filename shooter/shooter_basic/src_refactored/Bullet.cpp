#include "../include/Bullet.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity)
    : m_velocity(velocity)
{
    m_shape.setRadius(5);
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setOrigin({5, 5});
    m_shape.setPosition(position);
}

void Bullet::update(float deltaTime) {
    m_shape.move(m_velocity * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

bool Bullet::isOffScreen() const {
    sf::Vector2f pos = m_shape.getPosition();
    return pos.x < -10 || pos.x > 810 || pos.y < -10 || pos.y > 610;
}

sf::FloatRect Bullet::getBounds() const {
    return m_shape.getGlobalBounds();
}
