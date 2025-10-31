#include "Hammer.h"
#include <iostream>

Hammer::Hammer(sf::Vector2f playerPos, float radius)
    : m_angle(0.0f)
    , m_rotationSpeed(12.56637f)  // 720도/초 = 4π 라디안/초 (매우 빠르게!)
    , m_radius(radius)
    , m_position(playerPos)
    , m_totalRotation(0.0f)
    , m_active(false)
    , m_finished(true)
{
    // 손잡이 설정 (플레이어에서 시작, 길이 150까지)
    m_handle.setSize(sf::Vector2f(150.0f, 20.0f));
    m_handle.setFillColor(sf::Color(150, 150, 150));  // 회색
    m_handle.setOrigin(0.0f, 10.0f);  // 손잡이 시작점이 회전 중심 (플레이어 위치)

    // 망치 머리 설정 (정사각형 70x70, 손잡이 끝에 위치)
    m_head.setSize(sf::Vector2f(70.0f, 70.0f));
    m_head.setFillColor(sf::Color(180, 180, 180));  // 밝은 회색
    m_head.setOrigin(-150.0f, 35.0f);  // 손잡이 끝에 위치하도록
}

void Hammer::activate() {
    if (m_finished) {  // 이전 스킬이 끝났을 때만 다시 사용 가능
        m_active = true;
        m_finished = false;
        m_totalRotation = 0.0f;
        m_angle = 0.0f;  // 각도 초기화
        std::cout << "Hammer activated!" << std::endl;  // 디버그 출력
    }
}

void Hammer::update(float deltaTime, sf::Vector2f playerPos) {
    if (!m_active) return;

    // 각도 업데이트
    float rotationDelta = m_rotationSpeed * deltaTime;
    m_angle += rotationDelta;
    m_totalRotation += rotationDelta;

    // 한 바퀴 (2π) 이상 회전하면 스킬 종료
    if (m_totalRotation >= 2.0f * 3.14159f) {
        m_active = false;
        m_finished = true;
        m_totalRotation = 0.0f;
        return;
    }

    // 망치는 플레이어 위치에서 시작해서 회전
    m_position = playerPos;

    // 망치 회전 설정
    float degrees = m_angle * 180.0f / 3.14159f;
    m_handle.setPosition(m_position);
    m_handle.setRotation(degrees);

    m_head.setPosition(m_position);
    m_head.setRotation(degrees);
}

void Hammer::draw(sf::RenderWindow& window) {
    if (m_active) {  // 활성화 되었을 때만 그리기
        window.draw(m_handle);
        window.draw(m_head);
    }
}

sf::FloatRect Hammer::getBounds() const {
    // 망치 머리의 충돌 범위 반환 (하위 호환성)
    return m_head.getGlobalBounds();
}

sf::FloatRect Hammer::getHandleBounds() const {
    // 손잡이의 충돌 범위 반환
    return m_handle.getGlobalBounds();
}

sf::FloatRect Hammer::getHeadBounds() const {
    // 망치 머리의 충돌 범위 반환
    return m_head.getGlobalBounds();
}
