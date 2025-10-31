#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Hammer {
public:
    Hammer(sf::Vector2f playerPos, float radius = 150.0f);

    void activate();  // 망치 스킬 활성화
    void update(float deltaTime, sf::Vector2f playerPos);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::FloatRect getHandleBounds() const;  // 손잡이 충돌 범위
    sf::FloatRect getHeadBounds() const;    // 머리 충돌 범위
    bool isActive() const { return m_active; }
    bool isFinished() const { return m_finished; }

private:
    float m_angle;              // 현재 회전 각도 (라디안)
    float m_rotationSpeed;      // 회전 속도 (라디안/초) - 빠르게!
    float m_radius;             // 플레이어로부터의 거리
    float m_totalRotation;      // 총 회전한 각도
    bool m_active;              // 스킬이 활성화 되었는가
    bool m_finished;            // 한 바퀴 회전 완료

    sf::RectangleShape m_handle;    // 손잡이
    sf::RectangleShape m_head;      // 망치 머리
    sf::Vector2f m_position;
};
