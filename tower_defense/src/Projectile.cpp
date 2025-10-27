#include "../include/Projectile.h"
#include "../include/Enemy.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f position, Enemy* target, float damage)
    : m_target(target)
    , m_speed(300.0f)
    , m_damage(damage)
    , m_shouldRemove(false)
{
    m_shape.setRadius(3);
    m_shape.setOrigin({3, 3});
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setPosition(position);
}

void Projectile::update(float deltaTime) {
    if (!m_target || m_target->isDead()) {
        m_shouldRemove = true;
        return;
    }

    sf::Vector2f targetPos = m_target->getPosition();
    sf::Vector2f currentPos = m_shape.getPosition();
    sf::Vector2f direction = targetPos - currentPos;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        // Hit target
        m_target->takeDamage(static_cast<int>(m_damage));
        m_shouldRemove = true;
    } else {
        direction /= distance;  // Normalize
        m_shape.move(direction * m_speed * deltaTime);
    }
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}
