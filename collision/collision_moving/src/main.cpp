#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Collision - Moving with Keyboard");

    // 키보드로 움직이는 플레이어
    sf::RectangleShape player({40, 40});
    player.setFillColor(sf::Color::Green);
    player.setPosition({100, 100});

    // 장애물들
    sf::RectangleShape wall1({600, 20});
    wall1.setFillColor(sf::Color::Red);
    wall1.setPosition({100, 300});

    sf::RectangleShape wall2({20, 200});
    wall2.setFillColor(sf::Color::Red);
    wall2.setPosition({400, 100});

    sf::RectangleShape wall3({150, 150});
    wall3.setFillColor(sf::Color::Blue);
    wall3.setPosition({550, 400});

    float speed = 5.0f;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 이전 위치 저장
        sf::Vector2f previousPos = player.getPosition();

        // 키보드 입력으로 이동
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            player.move({-speed, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            player.move({speed, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            player.move({0, -speed});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            player.move({0, speed});
        }

        // 충돌 감지 - 벽과 부딪히면 이전 위치로 되돌리기
        sf::FloatRect playerBounds = player.getGlobalBounds();

        bool collision = false;
        if (playerBounds.findIntersection(wall1.getGlobalBounds()).has_value()) {
            collision = true;
        }
        if (playerBounds.findIntersection(wall2.getGlobalBounds()).has_value()) {
            collision = true;
        }
        if (playerBounds.findIntersection(wall3.getGlobalBounds()).has_value()) {
            collision = true;
        }

        // 화면 밖으로 나가는 것도 방지
        if (playerBounds.position.x < 0 ||
            playerBounds.position.x + playerBounds.size.x > 800 ||
            playerBounds.position.y < 0 ||
            playerBounds.position.y + playerBounds.size.y > 600) {
            collision = true;
        }

        // 충돌이 발생하면 이전 위치로 되돌림
        if (collision) {
            player.setPosition(previousPos);
        }

        window.clear(sf::Color::White);
        window.draw(wall1);
        window.draw(wall2);
        window.draw(wall3);
        window.draw(player);
        window.display();
    }

    return 0;
}
