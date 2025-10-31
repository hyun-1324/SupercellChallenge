#include "Enemy.h"
#include <cmath>

Enemy::Enemy(float x, float y)
    : m_speed(120.0f)  // 80에서 120으로 증가 (1.5배)
    , m_alive(true)
{
    // 삼각형 모양 (빨간색)
    m_shape.setPointCount(3);
    m_shape.setPoint(0, sf::Vector2f(0.0f, -20.0f));
    m_shape.setPoint(1, sf::Vector2f(-15.0f, 20.0f));
    m_shape.setPoint(2, sf::Vector2f(15.0f, 20.0f));
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition(x, y);
}

void Enemy::update(float deltaTime, sf::Vector2f playerPos) {
    if (!m_alive) return;

    // 플레이어를 향해 이동
    sf::Vector2f direction = playerPos - m_shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0.0f) {
        direction /= length;
        m_shape.move(direction * m_speed * deltaTime);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (m_alive) {
        window.draw(m_shape);
    }
}
