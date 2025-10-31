#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

class Player {
public:
    Player(float x, float y);

    void update(float deltaTime);
    void handleInput(const sf::Event& event, const sf::RenderWindow& window);
    void updateMousePosition(const sf::RenderWindow& window);  // 마우스 위치로 방향 업데이트
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

    bool shouldShoot() const { return m_shouldShoot; }
    void resetShoot() { m_shouldShoot = false; }
    sf::Vector2f getShootDirection() const { return m_shootDirection; }

    bool shouldActivateHammer() const { return m_shouldActivateHammer; }
    void resetHammerActivation() { m_shouldActivateHammer = false; }

    bool isDashing() const { return m_isDashing; }
    sf::FloatRect getDashHitbox() const;
    sf::Vector2f getFacingDirection() const { return m_facingDirection; }

    // 포인트 관리
    int getPoints() const { return m_points; }
    void addPoints(int points) { m_points += points; }
    bool canUseHammer() const { return m_points >= 30; }
    bool canUseDash() const { return m_points >= 15; }
    void useHammer() { m_points -= 30; }
    void useDash() { m_points -= 15; }

    // 스코어 관리
    int getScore() const { return m_score; }
    void addScore(int score) { m_score += score; }

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
    float m_speed;

    bool m_movingUp;
    bool m_movingDown;
    bool m_movingLeft;
    bool m_movingRight;

    bool m_shouldShoot;
    bool m_shouldActivateHammer;
    sf::Vector2f m_shootDirection;
    float m_shootCooldown;
    float m_shootCooldownTime;

    // 대쉬 관련
    bool m_isDashing;
    float m_dashTimer;
    float m_dashDuration;
    float m_dashSpeed;
    sf::Vector2f m_dashDirection;
    sf::Vector2f m_facingDirection;  // 플레이어가 바라보는 방향
    sf::RectangleShape m_dashHitbox;  // 대쉬 중 충돌 판정

    // 포인트와 스코어
    int m_points;
    int m_score;
};
