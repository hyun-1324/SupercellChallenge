#include "../include/Enemy.h"
#include <cmath>

Enemy::Enemy(std::vector<sf::Vector2i> path, int tileSize)
    : m_currentWaypoint(0)
    , m_speed(100.0f)
    , m_hp(100)
    , m_maxHp(100)
    , m_reachedEnd(false)
{
    // Convert grid path to world coordinates
    for (const auto& gridPos : path) {
        float x = gridPos.x * tileSize + tileSize / 2.0f;
        float y = gridPos.y * tileSize + tileSize / 2.0f;
        m_path.push_back({x, y});
    }

    m_shape.setRadius(8);
    m_shape.setOrigin({8, 8});
    m_shape.setFillColor(sf::Color::Red);

    if (!m_path.empty()) {
        m_shape.setPosition(m_path[0]);
    }
}

void Enemy::update(float deltaTime) {
    if (m_reachedEnd || m_path.empty() || m_currentWaypoint >= m_path.size()) {
        return;
    }

    sf::Vector2f target = m_path[m_currentWaypoint];
    sf::Vector2f current = m_shape.getPosition();
    sf::Vector2f direction = target - current;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        m_currentWaypoint++;
        if (m_currentWaypoint >= m_path.size()) {
            m_reachedEnd = true;
        }
    } else {
        direction /= distance;  // Normalize
        m_shape.move(direction * m_speed * deltaTime);
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);

    // HP bar
    sf::RectangleShape hpBar({16, 3});
    hpBar.setPosition({m_shape.getPosition().x - 8, m_shape.getPosition().y - 15});
    hpBar.setFillColor(sf::Color::Red);
    window.draw(hpBar);

    float hpPercent = static_cast<float>(m_hp) / m_maxHp;
    sf::RectangleShape hpFill({16 * hpPercent, 3});
    hpFill.setPosition({m_shape.getPosition().x - 8, m_shape.getPosition().y - 15});
    hpFill.setFillColor(sf::Color::Green);
    window.draw(hpFill);
}

void Enemy::takeDamage(int damage) {
    m_hp -= damage;
    if (m_hp < 0) m_hp = 0;
    updateColor();
}

void Enemy::updateColor() {
    float hpPercent = static_cast<float>(m_hp) / m_maxHp;
    std::uint8_t green = static_cast<std::uint8_t>(hpPercent * 100);
    m_shape.setFillColor(sf::Color(255, green, 0));
}
