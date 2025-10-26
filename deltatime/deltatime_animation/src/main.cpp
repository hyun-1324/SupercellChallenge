#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "DeltaTime - Animation");

    // 회전하는 프로펠러
    sf::RectangleShape propeller({100, 20});
    propeller.setFillColor(sf::Color::Red);
    propeller.setOrigin({50, 10});
    propeller.setPosition({200, 150});

    // 위아래로 움직이는 공
    sf::CircleShape ball(30);
    ball.setFillColor(sf::Color::Green);
    ball.setOrigin({30, 30});
    ball.setPosition({400, 300});

    // 크기가 변하는 하트
    sf::CircleShape heart(40);
    heart.setFillColor(sf::Color::Magenta);
    heart.setOrigin({40, 40});
    heart.setPosition({600, 150});

    // 애니메이션 변수
    float rotationSpeed = 180.0f;     // 초당 180도 회전
    float bounceSpeed = 200.0f;       // 초당 200픽셀 이동
    float ballDirection = 1.0f;       // 1 = 아래, -1 = 위
    float scaleSpeed = 1.0f;          // 초당 1배 변화
    float currentScale = 1.0f;
    float scaleDirection = 1.0f;      // 1 = 커짐, -1 = 작아짐

    // 시간 측정
    sf::Clock clock;
    float totalTime = 0.0f;

    while (window.isOpen()) {
        // ⭐ DeltaTime
        float deltaTime = clock.restart().asSeconds();
        totalTime += deltaTime;

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 1. 회전 애니메이션 (DeltaTime 적용)
        propeller.rotate(sf::degrees(rotationSpeed * deltaTime));

        // 2. 위아래 이동 애니메이션 (DeltaTime 적용)
        ball.move({0, bounceSpeed * deltaTime * ballDirection});

        // 위아래 반복
        if (ball.getPosition().y > 500) {
            ballDirection = -1.0f;
        }
        if (ball.getPosition().y < 100) {
            ballDirection = 1.0f;
        }

        // 3. 크기 변화 애니메이션 (DeltaTime 적용)
        currentScale += scaleSpeed * deltaTime * scaleDirection;

        // 크기 제한
        if (currentScale > 1.5f) {
            scaleDirection = -1.0f;
        }
        if (currentScale < 0.5f) {
            scaleDirection = 1.0f;
        }

        heart.setScale({currentScale, currentScale});

        // 4. 사인파 움직임 (시간 기반)
        // sin() 함수로 부드러운 움직임
        float wave = std::sin(totalTime * 2.0f) * 50.0f;  // -50 ~ +50 범위
        heart.setPosition({600 + wave, 150});

        window.clear(sf::Color::Black);
        window.draw(propeller);
        window.draw(ball);
        window.draw(heart);
        window.display();
    }

    return 0;
}

/*
DeltaTime을 사용한 애니메이션 패턴:

1. 회전:
   sprite.rotate(sf::degrees(speed * deltaTime));

2. 이동:
   sprite.move({velocity * deltaTime, 0});

3. 크기 변화:
   scale += scaleSpeed * deltaTime;
   sprite.setScale({scale, scale});

4. 사인파 (부드러운 움직임):
   float wave = std::sin(totalTime * frequency) * amplitude;
   sprite.setPosition({x + wave, y});

핵심:
  - 모든 변화량에 deltaTime을 곱한다
  - 속도는 "초당 변화량"으로 정의
  - 결과: 프레임 레이트와 무관하게 일정한 애니메이션!
*/
