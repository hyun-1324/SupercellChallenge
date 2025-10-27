#include "../include/Tower.h"
#include "../include/Enemy.h"
#include "../include/Projectile.h"
#include <cmath>

Tower::Tower(sf::Vector2f position)
    : m_range(150.0f)
    , m_damage(25.0f)
    , m_fireRate(1.0f)
    , m_cooldown(0.0f)
{
    m_shape.setSize({30, 30});
    m_shape.setOrigin({15, 15});
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition(position);

    m_rangeCircle.setRadius(m_range);
    m_rangeCircle.setOrigin({m_range, m_range});
    m_rangeCircle.setPosition(position);
    m_rangeCircle.setFillColor(sf::Color::Transparent);
    m_rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 50));
    m_rangeCircle.setOutlineThickness(1);
}

void Tower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    m_cooldown -= deltaTime;

    // Update projectiles
    for (auto& projectile : m_projectiles) {
        projectile->update(deltaTime);
    }

    // Remove finished projectiles
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const std::unique_ptr<Projectile>& p) { return p->shouldRemove(); }),
        m_projectiles.end()
    );

    // Find and shoot target
    if (m_cooldown <= 0) {
        Enemy* target = findTarget(enemies);
        if (target) {
            shoot(target);
            m_cooldown = m_fireRate;
        }
    }
}

void Tower::draw(sf::RenderWindow& window) const {
    window.draw(m_rangeCircle);
    window.draw(m_shape);

    for (const auto& projectile : m_projectiles) {
        projectile->draw(window);
    }
}

Enemy* Tower::findTarget(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* closest = nullptr;
    float minDist = m_range;

    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        sf::Vector2f diff = enemyPos - m_shape.getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist <= m_range && dist < minDist) {
            minDist = dist;
            closest = enemy.get();
        }
    }

    return closest;
}

void Tower::shoot(Enemy* target) {
    m_projectiles.push_back(
        std::make_unique<Projectile>(m_shape.getPosition(), target, m_damage)
    );
}
