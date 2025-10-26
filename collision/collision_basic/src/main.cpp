#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Collision Detection - Basic");

    // 마우스로 움직일 수 있는 빨간 사각형
    sf::RectangleShape player({50, 50});
    player.setFillColor(sf::Color::Red);
    player.setPosition({100, 100});

    // 고정된 파란 사각형
    sf::RectangleShape obstacle({100, 100});
    obstacle.setFillColor(sf::Color::Blue);
    obstacle.setPosition({400, 300});

    bool isDragging = false;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // 마우스 버튼 pressed
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    // 마우스가 player 위에 있는지 확인
                    sf::Vector2f mousePos = sf::Vector2f(mousePressed->position);
                    if (player.getGlobalBounds().contains(mousePos)) {
                        isDragging = true;
                    }
                }
            }

            // 마우스 버튼 released
            if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseReleased->button == sf::Mouse::Button::Left) {
                    isDragging = false;
                }
            }
        }

        // 드래그 중이면 마우스 위치로 이동
        if (isDragging) {
            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
            player.setPosition(mousePos - sf::Vector2f{25, 25});
        }

        // 충돌 감지! (SFML 3.0)
        sf::FloatRect playerBounds = player.getGlobalBounds();
        sf::FloatRect obstacleBounds = obstacle.getGlobalBounds();

        // findIntersection을 사용 (교집합이 있으면 충돌)
        auto intersection = playerBounds.findIntersection(obstacleBounds);
        bool isColliding = intersection.has_value();

        // 충돌하면 색 변경
        if (isColliding) {
            player.setFillColor(sf::Color::Green);   // 충돌! 초록색
            obstacle.setFillColor(sf::Color::Yellow); // 충돌! 노란색
        } else {
            player.setFillColor(sf::Color::Red);      // 정상 - 빨간색
            obstacle.setFillColor(sf::Color::Blue);   // 정상 - 파란색
        }

        window.clear(sf::Color::White);
        window.draw(obstacle);
        window.draw(player);
        window.display();
    }

    return 0;
}
