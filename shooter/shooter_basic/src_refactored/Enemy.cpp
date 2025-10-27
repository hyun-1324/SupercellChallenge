#include "../include/Enemy.h"
#include <cmath>

Enemy::Enemy(sf::Vector2f position)
    : m_hp(3)
    , m_maxHp(3)
    , m_speed(100.0f)
{
    m_shape.setRadius(15);
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setOrigin({15, 15});
    m_shape.setPosition(position);
}

void Enemy::update(float deltaTime, sf::Vector2f playerPos) {
    // Move towards player
    sf::Vector2f direction = playerPos - m_shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0) {
        direction /= length;  // Normalize
        m_shape.move(direction * m_speed * deltaTime);
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

void Enemy::takeDamage(int damage) {
    m_hp -= damage;
    if (m_hp < 0) {
        m_hp = 0;
    }
    updateColor();
}

bool Enemy::isDead() const {
    return m_hp <= 0;
}

sf::FloatRect Enemy::getBounds() const {
    return m_shape.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const {
    return m_shape.getPosition();
}

void Enemy::updateColor() {
    if (m_hp == 2) {
        m_shape.setFillColor(sf::Color(255, 100, 100));
    } else if (m_hp == 1) {
        m_shape.setFillColor(sf::Color(150, 0, 0));
    }
}
