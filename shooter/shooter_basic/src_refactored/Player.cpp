#include "../include/Player.h"
#include "../include/Bullet.h"
#include <cmath>
#include <algorithm>

Player::Player()
    : m_speed(250.0f)
    , m_shootCooldown(0.0f)
    , m_shootInterval(0.2f)
{
    m_shape.setRadius(20);
    m_shape.setFillColor(sf::Color::Green);
    m_shape.setOrigin({20, 20});
    m_shape.setPosition({400, 300});

    m_aimLine.setSize({30, 2});
    m_aimLine.setFillColor(sf::Color::White);
    m_aimLine.setOrigin({0, 1});
}

void Player::handleInput(float deltaTime) {
    sf::Vector2f movement(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1;

    // Normalize diagonal movement
    float moveLength = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (moveLength > 0) {
        movement /= moveLength;
        m_shape.move(movement * m_speed * deltaTime);
    }

    // Clamp to screen bounds
    sf::Vector2f pos = m_shape.getPosition();
    pos.x = std::max(20.f, std::min(780.f, pos.x));
    pos.y = std::max(20.f, std::min(580.f, pos.y));
    m_shape.setPosition(pos);
}

void Player::update(float deltaTime) {
    if (m_shootCooldown > 0) {
        m_shootCooldown -= deltaTime;
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
    window.draw(m_aimLine);
}

void Player::updateAimLine(sf::Vector2f mousePos) {
    sf::Vector2f aimDir = mousePos - m_shape.getPosition();
    float angle = std::atan2(aimDir.y, aimDir.x) * 180.f / 3.14159f;
    m_aimLine.setPosition(m_shape.getPosition());
    m_aimLine.setRotation(sf::degrees(angle));
}

bool Player::canShoot() const {
    return m_shootCooldown <= 0;
}

void Player::shoot(sf::Vector2f mousePos) {
    // This method is deprecated - shooting is handled in Game class
}

void Player::resetShootCooldown() {
    m_shootCooldown = m_shootInterval;
}

sf::Vector2f Player::getPosition() const {
    return m_shape.getPosition();
}

sf::FloatRect Player::getBounds() const {
    return m_shape.getGlobalBounds();
}

void Player::pushBack(sf::Vector2f direction, float amount, float deltaTime) {
    m_shape.move(direction * amount * deltaTime);
}
