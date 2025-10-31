#include "Projectile.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f direction)
    : m_speed(400.0f)
{
    m_shape.setSize(sf::Vector2f(20.0f, 5.0f));
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setOrigin(10.0f, 2.5f);
    m_shape.setPosition(startPos);

    // 방향 정규화
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.0f) {
        direction /= length;
    }

    m_velocity = direction * m_speed;

    // 발사 방향으로 회전
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    m_shape.setRotation(angle);
}

void Projectile::update(float deltaTime) {
    m_shape.move(m_velocity * deltaTime);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
}

bool Projectile::isOffScreen(float screenWidth, float screenHeight) const {
    sf::Vector2f pos = m_shape.getPosition();
    return pos.x < -50.0f || pos.x > screenWidth + 50.0f ||
           pos.y < -50.0f || pos.y > screenHeight + 50.0f;
}
