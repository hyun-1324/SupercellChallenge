#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

// 두 원 사이의 거리 계산
float distance(sf::Vector2f pos1, sf::Vector2f pos2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dy * dy);
}

// 원형 충돌 감지
bool checkCircleCollision(const sf::CircleShape& circle1, const sf::CircleShape& circle2) {
    // 두 원의 중심 위치
    sf::Vector2f center1 = circle1.getPosition();
    sf::Vector2f center2 = circle2.getPosition();

    // 두 원의 반지름
    float radius1 = circle1.getRadius();
    float radius2 = circle2.getRadius();

    // 중심 간 거리
    float dist = distance(center1, center2);

    // 거리가 두 반지름의 합보다 작으면 충돌!
    return dist < (radius1 + radius2);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Collision - Circle (Distance-based)");

    // 마우스를 따라다니는 원
    sf::CircleShape player(30);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({30, 30});  // 중심을 origin으로
    player.setPosition({400, 300});

    // 장애물 원들
    sf::CircleShape obstacle1(50);
    obstacle1.setFillColor(sf::Color::Red);
    obstacle1.setOrigin({50, 50});
    obstacle1.setPosition({200, 200});

    sf::CircleShape obstacle2(40);
    obstacle2.setFillColor(sf::Color::Blue);
    obstacle2.setOrigin({40, 40});
    obstacle2.setPosition({600, 400});

    sf::CircleShape obstacle3(60);
    obstacle3.setFillColor(sf::Color::Magenta);
    obstacle3.setOrigin({60, 60});
    obstacle3.setPosition({400, 100});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 마우스 위치로 플레이어 이동
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        player.setPosition(mousePos);

        // 원형 충돌 감지
        bool collision1 = checkCircleCollision(player, obstacle1);
        bool collision2 = checkCircleCollision(player, obstacle2);
        bool collision3 = checkCircleCollision(player, obstacle3);

        // 충돌 시 색상 변경
        if (collision1) {
            obstacle1.setFillColor(sf::Color::Yellow);
        } else {
            obstacle1.setFillColor(sf::Color::Red);
        }

        if (collision2) {
            obstacle2.setFillColor(sf::Color::Cyan);
        } else {
            obstacle2.setFillColor(sf::Color::Blue);
        }

        if (collision3) {
            obstacle3.setFillColor(sf::Color::White);
        } else {
            obstacle3.setFillColor(sf::Color::Magenta);
        }

        // 플레이어 색상 (충돌하면 빨강)
        if (collision1 || collision2 || collision3) {
            player.setFillColor(sf::Color::Red);
        } else {
            player.setFillColor(sf::Color::Green);
        }

        window.clear(sf::Color::Black);
        window.draw(obstacle1);
        window.draw(obstacle2);
        window.draw(obstacle3);
        window.draw(player);
        window.display();
    }

    return 0;
}
