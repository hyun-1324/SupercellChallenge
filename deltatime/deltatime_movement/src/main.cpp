#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "DeltaTime - Movement");

    // 플레이어
    sf::CircleShape player(25);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({25, 25});
    player.setPosition({400, 300});

    // 속도 (픽셀/초)
    float moveSpeed = 300.0f;  // 초당 300픽셀

    // DeltaTime 측정
    sf::Clock clock;

    while (window.isOpen()) {
        // ⭐ DeltaTime 계산 - 이전 프레임 이후 경과 시간
        float deltaTime = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 이동 벡터 (방향)
        sf::Vector2f movement(0.f, 0.f);

        // 키보드 입력 - 방향 설정
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            movement.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            movement.x += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            movement.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            movement.y += 1.0f;
        }

        // ⭐ DeltaTime 적용!
        // movement = 방향 벡터
        // moveSpeed = 속도 (픽셀/초)
        // deltaTime = 시간 (초)
        // 결과 = 픽셀
        player.move(movement * moveSpeed * deltaTime);

        // 화면 밖으로 나가지 않게
        sf::Vector2f pos = player.getPosition();
        if (pos.x < 25) pos.x = 25;
        if (pos.x > 775) pos.x = 775;
        if (pos.y < 25) pos.y = 25;
        if (pos.y > 575) pos.y = 575;
        player.setPosition(pos);

        window.clear(sf::Color::Black);
        window.draw(player);
        window.display();
    }

    return 0;
}

/*
핵심 공식:

  player.move(direction * speed * deltaTime);

  - direction: 방향 벡터 (예: {1, 0} = 오른쪽)
  - speed: 속도 (픽셀/초)
  - deltaTime: 이전 프레임 이후 경과 시간 (초)

예시:
  - 60 FPS일 때: deltaTime ≈ 0.0167초
    → movement = {1, 0} * 300 * 0.0167 = {5, 0} 픽셀

  - 30 FPS일 때: deltaTime ≈ 0.0333초
    → movement = {1, 0} * 300 * 0.0333 = {10, 0} 픽셀

결과: 둘 다 초당 300픽셀로 이동! ✅
*/
