#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Collision - Sprite");

    // 텍스처 로드
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../assets/player.png")) {
        std::cout << "이미지 로드 실패!" << std::endl;
        return -1;
    }

    std::cout << "텍스처 크기: " << playerTexture.getSize().x << " x " << playerTexture.getSize().y << std::endl;

    // 플레이어 스프라이트 (전체 텍스처 사용)
    sf::Sprite player(playerTexture);
    // TextureRect 설정 안 하면 자동으로 전체 텍스처 사용
    player.setScale({1.1f, 1.1f});  // 64x64를 128x128로
    player.setPosition({100, 250});

    // 적 스프라이트들
    std::vector<sf::Sprite> enemies;
    for (int i = 0; i < 5; ++i) {
        sf::Sprite enemy(playerTexture);
        // TextureRect 설정 안 함 = 전체 텍스처 사용
        enemy.setScale({1.0f, 1.0f});  // 64x64를 96x96로
        enemy.setPosition({400.f + i * 80.f, 100.f + i * 60.f});
        enemy.setColor(sf::Color::Red);  // 빨간색으로 구분
        enemies.push_back(enemy);
    }

    std::cout << "적 개수: " << enemies.size() << std::endl;

    int lives = 3;
    float speed = 5.0f;
    bool invincible = false;
    sf::Clock invincibleTimer;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 플레이어 이동
        sf::Vector2f previousPos = player.getPosition();

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

        // 화면 밖으로 나가는 것 방지
        sf::FloatRect playerBounds = player.getGlobalBounds();
        if (playerBounds.position.x < 0 ||
            playerBounds.position.x + playerBounds.size.x > 800 ||
            playerBounds.position.y < 0 ||
            playerBounds.position.y + playerBounds.size.y > 600) {
            player.setPosition(previousPos);
        }

        // 무적 시간 해제
        if (invincible && invincibleTimer.getElapsedTime().asSeconds() > 2.0f) {
            invincible = false;
        }

        // 적과 충돌 감지
        if (!invincible) {
            for (const auto& enemy : enemies) {
                if (playerBounds.findIntersection(enemy.getGlobalBounds()).has_value()) {
                    lives--;
                    invincible = true;
                    invincibleTimer.restart();
                    std::cout << "충돌! 남은 생명: " << lives << std::endl;

                    if (lives <= 0) {
                        std::cout << "게임 오버!" << std::endl;
                        window.close();
                    }
                    break;
                }
            }
        }

        // 그리기
        window.clear(sf::Color::Black);  // 검은색 배경으로 변경

        // 적 그리기
        for (const auto& enemy : enemies) {
            window.draw(enemy);
        }

        // 플레이어 그리기 (무적이면 깜빡임)
        if (!invincible || (static_cast<int>(invincibleTimer.getElapsedTime().asMilliseconds() / 200) % 2 == 0)) {
            window.draw(player);
        }

        window.display();
    }

    return 0;
}
