#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Collision - Multiple Objects");

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setPosition({400, 300});
    player.setOrigin({20, 20});

    // 수집할 아이템들 (동전)
    std::vector<sf::CircleShape> coins;
    for (int i = 0; i < 10; ++i) {
        sf::CircleShape coin(15);
        coin.setFillColor(sf::Color::Yellow);
        coin.setPosition({100.f + i * 70.f, 100.f});
        coin.setOrigin({15, 15});
        coins.push_back(coin);
    }

    for (int i = 0; i < 10; ++i) {
        sf::CircleShape coin(15);
        coin.setFillColor(sf::Color::Yellow);
        coin.setPosition({100.f + i * 70.f, 400.f});
        coin.setOrigin({15, 15});
        coins.push_back(coin);
    }

    int score = 0;
    float speed = 4.0f;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 플레이어 이동
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

        // 여러 동전들과 충돌 감지 (Iterator 방식)
        sf::FloatRect playerBounds = player.getGlobalBounds();
        bool coinCollected = false;

        for (auto it = coins.begin(); it != coins.end(); ) {
            if (playerBounds.findIntersection(it->getGlobalBounds()).has_value()) {
                // 점수 증가
                score += 10;
                coinCollected = true;

                // 동전 삭제 (erase는 다음 iterator 반환)
                it = coins.erase(it);
            } else {
                // 충돌 없으면 다음으로
                ++it;
            }
        }

        // 동전을 먹었으면 메시지 출력
        if (coinCollected) {
            std::cout << "Score: " << score << " (남은 동전: " << coins.size() << ")" << std::endl;
        }

        // 모든 동전을 먹었으면
        if (coins.empty()) {
            std::cout << "축하합니다! 모든 동전을 수집했습니다!" << std::endl;
            std::cout << "최종 점수: " << score << std::endl;
            window.close();
        }

        window.clear(sf::Color::Black);

        // 동전들 그리기
        for (const auto& coin : coins) {
            window.draw(coin);
        }

        window.draw(player);
        window.display();
    }

    return 0;
}
