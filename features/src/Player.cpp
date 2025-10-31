#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(float x, float y)
    : m_speed(200.0f)
    , m_movingUp(false)
    , m_movingDown(false)
    , m_movingLeft(false)
    , m_movingRight(false)
    , m_shouldShoot(false)
    , m_shouldActivateHammer(false)
    , m_shootDirection(0.0f, 0.0f)
    , m_shootCooldown(0.0f)
    , m_shootCooldownTime(0.5f)  // 0.5초마다 발사 가능
    , m_isDashing(false)
    , m_dashTimer(0.0f)
    , m_dashDuration(0.25f)  // 0.25초 동안 대쉬
    , m_dashSpeed(1000.0f)  // 대쉬 속도 (매우 빠르게!)
    , m_dashDirection(0.0f, 0.0f)
    , m_facingDirection(1.0f, 0.0f)  // 기본은 오른쪽
    , m_points(0)  // 초기 포인트 0
    , m_score(0)   // 초기 스코어 0
{
    m_shape.setRadius(25.0f);
    m_shape.setFillColor(sf::Color::Blue);
    m_shape.setOrigin(25.0f, 25.0f);
    m_shape.setPosition(x, y);

    // 대쉬 히트박스 설정
    m_dashHitbox.setSize(sf::Vector2f(80.0f, 50.0f));
    m_dashHitbox.setFillColor(sf::Color(255, 255, 255, 100));  // 반투명 흰색
    m_dashHitbox.setOrigin(0.0f, 25.0f);
}

void Player::update(float deltaTime) {
    // 쿨다운 업데이트
    if (m_shootCooldown > 0.0f) {
        m_shootCooldown -= deltaTime;
    }

    // 대쉬 중이면
    if (m_isDashing) {
        m_dashTimer -= deltaTime;

        if (m_dashTimer <= 0.0f) {
            // 대쉬 종료 - 반대 방향으로 전환
            m_isDashing = false;
            m_facingDirection = -m_dashDirection;
            std::cout << "Dash ended! Now facing opposite direction" << std::endl;
        } else {
            // 대쉬 방향으로 빠르게 이동
            m_shape.move(m_dashDirection * m_dashSpeed * deltaTime);

            // 대쉬 히트박스 업데이트
            float angle = std::atan2(m_dashDirection.y, m_dashDirection.x) * 180.0f / 3.14159f;
            m_dashHitbox.setPosition(m_shape.getPosition());
            m_dashHitbox.setRotation(angle);
        }
        return;
    }

    // 일반 이동
    m_velocity = sf::Vector2f(0.0f, 0.0f);

    if (m_movingUp) m_velocity.y -= 1.0f;
    if (m_movingDown) m_velocity.y += 1.0f;
    if (m_movingLeft) m_velocity.x -= 1.0f;
    if (m_movingRight) m_velocity.x += 1.0f;

    // 정규화
    float length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
    if (length != 0.0f) {
        m_velocity /= length;
    }

    m_shape.move(m_velocity * m_speed * deltaTime);
}

void Player::updateMousePosition(const sf::RenderWindow& window) {
    // 마우스 커서 방향으로 바라보는 방향 업데이트
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    sf::Vector2f direction = worldPos - m_shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 10.0f) {  // 마우스가 플레이어에서 충분히 멀리 있을 때만
        m_facingDirection = direction / length;
    }
}

void Player::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) m_movingUp = true;
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) m_movingDown = true;
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) m_movingLeft = true;
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) m_movingRight = true;

        // E 키로 망치 스킬 활성화
        if (event.key.code == sf::Keyboard::E) {
            if (canUseHammer()) {
                m_shouldActivateHammer = true;
                std::cout << "E key pressed! Points: " << m_points << std::endl;
            } else {
                std::cout << "Not enough points for Hammer! Need 30, have " << m_points << std::endl;
            }
        }

        // R 키로 대쉬
        if (event.key.code == sf::Keyboard::R) {
            if (!m_isDashing && m_facingDirection != sf::Vector2f(0.0f, 0.0f) && canUseDash()) {
                m_isDashing = true;
                m_dashTimer = m_dashDuration;
                m_dashDirection = m_facingDirection;
                useDash();  // 포인트 차감
                std::cout << "Dash activated! Points remaining: " << m_points << std::endl;
            } else if (!canUseDash()) {
                std::cout << "Not enough points for Dash! Need 15, have " << m_points << std::endl;
            }
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) m_movingUp = false;
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) m_movingDown = false;
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) m_movingLeft = false;
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) m_movingRight = false;
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // 쿨다운이 끝났을 때만 발사 가능
            if (m_shootCooldown <= 0.0f) {
                // 마우스 클릭으로 미사일 발사
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                m_shootDirection = worldPos - m_shape.getPosition();
                m_shouldShoot = true;
                m_shootCooldown = m_shootCooldownTime;  // 쿨다운 시작
            }
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    // 대쉬 중이면 히트박스도 그리기
    if (m_isDashing) {
        window.draw(m_dashHitbox);
    }
    window.draw(m_shape);
}

sf::FloatRect Player::getDashHitbox() const {
    return m_dashHitbox.getGlobalBounds();
}
