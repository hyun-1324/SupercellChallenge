#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Top-Down Shooter Basic - SFML 3.0
//
// 핵심 메커니즘:
// 1. 마우스 방향으로 조준
// 2. 클릭으로 총알 발사
// 3. 총알이 적을 맞추면 제거
// 4. 적이 플레이어를 향해 이동
//
// 게임잼 핵심 패턴!

struct Bullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Bullet(sf::Vector2f pos, sf::Vector2f vel) {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin({5, 5});
        shape.setPosition(pos);
        velocity = vel;
    }
};

struct Enemy {
    sf::CircleShape shape;
    int hp;

    Enemy(sf::Vector2f pos) {
        shape.setRadius(15);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin({15, 15});
        shape.setPosition(pos);
        hp = 3;
    }
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Top-Down Shooter Basic - SFML 3.0");
    window.setFramerateLimit(60);

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});
    player.setPosition({400, 300});
    float playerSpeed = 250.0f;

    // 조준선
    sf::RectangleShape aimLine({30, 2});
    aimLine.setFillColor(sf::Color::White);
    aimLine.setOrigin({0, 1});

    // 총알 & 적
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    // 게임 데이터
    int score = 0;
    float enemySpawnTimer = 0.0f;
    const float enemySpawnInterval = 2.0f;
    float shootCooldown = 0.0f;
    const float shootInterval = 0.2f;  // 초당 5발

    // UI
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text scoreText(font, "Score: 0", 24);
    scoreText.setPosition({10, 10});
    scoreText.setFillColor(sf::Color::White);

    sf::Text enemyCountText(font, "Enemies: 0", 20);
    enemyCountText.setPosition({10, 50});
    enemyCountText.setFillColor(sf::Color::Yellow);

    sf::Text controlsText(font, "WASD: Move | Mouse: Aim | Click: Shoot", 16);
    controlsText.setPosition({200, 570});
    controlsText.setFillColor(sf::Color(150, 150, 150));

    sf::Clock deltaClock;
    sf::Vector2f mousePos;

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();

        // 마우스 위치 (월드 좌표)
        mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // 총알 발사 (클릭)
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left && shootCooldown <= 0) {
                    // 플레이어에서 마우스로 향하는 방향
                    sf::Vector2f direction = mousePos - player.getPosition();
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                    if (length > 0) {
                        direction /= length;  // 정규화

                        float bulletSpeed = 500.0f;
                        bullets.emplace_back(
                            player.getPosition(),
                            direction * bulletSpeed
                        );

                        shootCooldown = shootInterval;
                    }
                }
            }
        }

        // 쿨다운
        if (shootCooldown > 0) {
            shootCooldown -= deltaTime;
        }

        // 플레이어 이동
        sf::Vector2f movement(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1;

        // 정규화 (대각선 이동 속도 보정)
        float moveLength = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        if (moveLength > 0) {
            movement /= moveLength;
            player.move(movement * playerSpeed * deltaTime);
        }

        // 화면 경계
        sf::Vector2f playerPos = player.getPosition();
        playerPos.x = std::max(20.f, std::min(780.f, playerPos.x));
        playerPos.y = std::max(20.f, std::min(580.f, playerPos.y));
        player.setPosition(playerPos);

        // 조준선 회전 (플레이어 → 마우스)
        sf::Vector2f aimDir = mousePos - player.getPosition();
        float angle = std::atan2(aimDir.y, aimDir.x) * 180.f / 3.14159f;
        aimLine.setPosition(player.getPosition());
        aimLine.setRotation(sf::degrees(angle));

        // 적 생성
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= enemySpawnInterval) {
            enemySpawnTimer = 0;

            // 화면 밖에서 생성
            int side = std::rand() % 4;
            sf::Vector2f spawnPos;

            switch (side) {
                case 0: spawnPos = {static_cast<float>(std::rand() % 800), -20.f}; break;  // 위
                case 1: spawnPos = {static_cast<float>(std::rand() % 800), 620.f}; break;  // 아래
                case 2: spawnPos = {-20.f, static_cast<float>(std::rand() % 600)}; break;  // 왼쪽
                case 3: spawnPos = {820.f, static_cast<float>(std::rand() % 600)}; break;  // 오른쪽
            }

            enemies.emplace_back(spawnPos);
        }

        // 총알 이동
        for (auto& bullet : bullets) {
            bullet.shape.move(bullet.velocity * deltaTime);
        }

        // 화면 밖 총알 제거
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) {
                    sf::Vector2f pos = b.shape.getPosition();
                    return pos.x < -10 || pos.x > 810 || pos.y < -10 || pos.y > 610;
                }),
            bullets.end()
        );

        // 적 이동 (플레이어 추적)
        for (auto& enemy : enemies) {
            sf::Vector2f enemyPos = enemy.shape.getPosition();
            sf::Vector2f direction = player.getPosition() - enemyPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length > 0) {
                direction /= length;
                enemy.shape.move(direction * 100.0f * deltaTime);
            }
        }

        // 총알 vs 적 충돌
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
            bool bulletHit = false;

            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
                if (bulletIt->shape.getGlobalBounds().findIntersection(
                    enemyIt->shape.getGlobalBounds()).has_value()) {

                    // 적 HP 감소
                    enemyIt->hp--;

                    if (enemyIt->hp <= 0) {
                        score += 10;
                        scoreText.setString("Score: " + std::to_string(score));
                        enemyIt = enemies.erase(enemyIt);
                    } else {
                        // HP 남았으면 색 변경
                        if (enemyIt->hp == 2) {
                            enemyIt->shape.setFillColor(sf::Color(255, 100, 100));
                        } else if (enemyIt->hp == 1) {
                            enemyIt->shape.setFillColor(sf::Color(150, 0, 0));
                        }
                        ++enemyIt;
                    }

                    bulletHit = true;
                    break;
                } else {
                    ++enemyIt;
                }
            }

            if (bulletHit) {
                bulletIt = bullets.erase(bulletIt);
            } else {
                ++bulletIt;
            }
        }

        // 적 vs 플레이어 충돌 (게임오버는 나중에)
        for (const auto& enemy : enemies) {
            if (player.getGlobalBounds().findIntersection(enemy.shape.getGlobalBounds()).has_value()) {
                // 간단하게 적 밀어내기
                sf::Vector2f pushDir = player.getPosition() - enemy.shape.getPosition();
                float pushLength = std::sqrt(pushDir.x * pushDir.x + pushDir.y * pushDir.y);
                if (pushLength > 0) {
                    pushDir /= pushLength;
                    player.move(pushDir * 200.0f * deltaTime);
                }
            }
        }

        enemyCountText.setString("Enemies: " + std::to_string(enemies.size()));

        // 렌더링
        window.clear(sf::Color::Black);

        // 게임 오브젝트
        window.draw(player);
        window.draw(aimLine);

        for (const auto& bullet : bullets) {
            window.draw(bullet.shape);
        }

        for (const auto& enemy : enemies) {
            window.draw(enemy.shape);
        }

        // UI
        window.draw(scoreText);
        window.draw(enemyCountText);
        window.draw(controlsText);

        window.display();
    }

    return 0;
}
