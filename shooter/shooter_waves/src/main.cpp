#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Top-Down Shooter Waves - SFML 3.0
//
// 웨이브 시스템:
// - 웨이브 단위로 적 생성
// - 모든 적 처치 → 다음 웨이브
// - 웨이브마다 적 증가
// - 웨이브 사이 휴식 시간
//
// 타워디펜스/생존 게임 핵심!

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
    float speed;

    Enemy(sf::Vector2f pos, int wave) {
        shape.setRadius(15);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin({15, 15});
        shape.setPosition(pos);
        hp = 2 + wave;  // 웨이브마다 HP 증가
        speed = 80.0f + wave * 10.0f;  // 웨이브마다 빨라짐
    }
};

enum class GameState {
    Playing,
    WaveComplete,
    GameOver
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Shooter Waves - SFML 3.0");
    window.setFramerateLimit(60);

    GameState gameState = GameState::Playing;

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});
    player.setPosition({400, 300});
    float playerSpeed = 250.0f;
    int playerHP = 100;

    // 조준선
    sf::RectangleShape aimLine({30, 2});
    aimLine.setFillColor(sf::Color::White);
    aimLine.setOrigin({0, 1});

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    // 웨이브 시스템
    int currentWave = 1;
    int enemiesThisWave = 5;
    int enemiesSpawned = 0;
    float spawnTimer = 0.0f;
    float spawnInterval = 1.5f;
    float waveCompleteTimer = 0.0f;
    const float waveCompleteDelay = 3.0f;

    int score = 0;
    float shootCooldown = 0.0f;
    const float shootInterval = 0.15f;
    float invincibilityTimer = 0.0f;

    // UI
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text waveText(font, "Wave 1", 32);
    waveText.setPosition({10, 10});
    waveText.setFillColor(sf::Color::Yellow);

    sf::Text scoreText(font, "Score: 0", 20);
    scoreText.setPosition({10, 50});
    scoreText.setFillColor(sf::Color::White);

    sf::Text hpText(font, "HP: 100", 20);
    hpText.setPosition({10, 80});
    hpText.setFillColor(sf::Color::Green);

    sf::Text enemyText(font, "Enemies: 0/5", 18);
    enemyText.setPosition({10, 110});
    enemyText.setFillColor(sf::Color::Cyan);

    sf::Text waveCompleteText(font, "WAVE COMPLETE!", 48);
    waveCompleteText.setPosition({200, 250});
    waveCompleteText.setFillColor(sf::Color::Yellow);

    sf::Text nextWaveText(font, "Next wave in 3...", 24);
    nextWaveText.setPosition({280, 320});
    nextWaveText.setFillColor(sf::Color::White);

    sf::Text gameOverText(font, "GAME OVER", 56);
    gameOverText.setPosition({220, 220});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text restartText(font, "Press R to Restart", 24);
    restartText.setPosition({260, 320});
    restartText.setFillColor(sf::Color::White);

    sf::Clock deltaClock;

    auto resetGame = [&]() {
        gameState = GameState::Playing;
        currentWave = 1;
        enemiesThisWave = 5;
        enemiesSpawned = 0;
        score = 0;
        playerHP = 100;
        player.setPosition({400, 300});
        bullets.clear();
        enemies.clear();
        invincibilityTimer = 0.0f;
        waveText.setString("Wave 1");
        scoreText.setString("Score: 0");
        hpText.setString("HP: 100");
        hpText.setFillColor(sf::Color::Green);
    };

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left &&
                    shootCooldown <= 0 && gameState == GameState::Playing) {

                    sf::Vector2f direction = mousePos - player.getPosition();
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                    if (length > 0) {
                        direction /= length;
                        bullets.emplace_back(player.getPosition(), direction * 500.0f);
                        shootCooldown = shootInterval;
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::R && gameState == GameState::GameOver) {
                    resetGame();
                }
            }
        }

        if (shootCooldown > 0) shootCooldown -= deltaTime;
        if (invincibilityTimer > 0) invincibilityTimer -= deltaTime;

        if (gameState == GameState::Playing) {
            // 플레이어 이동
            sf::Vector2f movement(0, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1;

            float moveLength = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            if (moveLength > 0) {
                movement /= moveLength;
                player.move(movement * playerSpeed * deltaTime);
            }

            sf::Vector2f playerPos = player.getPosition();
            playerPos.x = std::max(20.f, std::min(780.f, playerPos.x));
            playerPos.y = std::max(20.f, std::min(580.f, playerPos.y));
            player.setPosition(playerPos);

            // 조준
            sf::Vector2f aimDir = mousePos - player.getPosition();
            float angle = std::atan2(aimDir.y, aimDir.x) * 180.f / 3.14159f;
            aimLine.setPosition(player.getPosition());
            aimLine.setRotation(sf::degrees(angle));

            // 웨이브 적 생성
            if (enemiesSpawned < enemiesThisWave) {
                spawnTimer += deltaTime;
                if (spawnTimer >= spawnInterval) {
                    spawnTimer = 0;

                    int side = std::rand() % 4;
                    sf::Vector2f spawnPos;
                    switch (side) {
                        case 0: spawnPos = {static_cast<float>(std::rand() % 800), -20.f}; break;
                        case 1: spawnPos = {static_cast<float>(std::rand() % 800), 620.f}; break;
                        case 2: spawnPos = {-20.f, static_cast<float>(std::rand() % 600)}; break;
                        case 3: spawnPos = {820.f, static_cast<float>(std::rand() % 600)}; break;
                    }

                    enemies.emplace_back(spawnPos, currentWave);
                    enemiesSpawned++;
                }
            }

            // 웨이브 완료 체크
            if (enemiesSpawned >= enemiesThisWave && enemies.empty()) {
                gameState = GameState::WaveComplete;
                waveCompleteTimer = waveCompleteDelay;
                std::cout << "Wave " << currentWave << " Complete!\n";
            }

            // 총알 이동
            for (auto& bullet : bullets) {
                bullet.shape.move(bullet.velocity * deltaTime);
            }

            bullets.erase(
                std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) {
                        sf::Vector2f pos = b.shape.getPosition();
                        return pos.x < -10 || pos.x > 810 || pos.y < -10 || pos.y > 610;
                    }),
                bullets.end()
            );

            // 적 이동
            for (auto& enemy : enemies) {
                sf::Vector2f enemyPos = enemy.shape.getPosition();
                sf::Vector2f direction = player.getPosition() - enemyPos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                if (length > 0) {
                    direction /= length;
                    enemy.shape.move(direction * enemy.speed * deltaTime);
                }
            }

            // 총알 vs 적
            for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
                bool hit = false;

                for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
                    if (bulletIt->shape.getGlobalBounds().findIntersection(
                        enemyIt->shape.getGlobalBounds()).has_value()) {

                        enemyIt->hp--;

                        if (enemyIt->hp <= 0) {
                            score += 10 * currentWave;
                            scoreText.setString("Score: " + std::to_string(score));
                            enemyIt = enemies.erase(enemyIt);
                        } else {
                            float hpPercent = static_cast<float>(enemyIt->hp) / (2.0f + currentWave);
                            std::uint8_t red = 255;
                            std::uint8_t green = static_cast<std::uint8_t>(hpPercent * 100);
                            enemyIt->shape.setFillColor(sf::Color(red, green, 0));
                            ++enemyIt;
                        }

                        hit = true;
                        break;
                    } else {
                        ++enemyIt;
                    }
                }

                if (hit) {
                    bulletIt = bullets.erase(bulletIt);
                } else {
                    ++bulletIt;
                }
            }

            // 적 vs 플레이어
            if (invincibilityTimer <= 0) {
                for (const auto& enemy : enemies) {
                    if (player.getGlobalBounds().findIntersection(
                        enemy.shape.getGlobalBounds()).has_value()) {

                        playerHP -= 10;
                        invincibilityTimer = 1.0f;
                        hpText.setString("HP: " + std::to_string(playerHP));

                        if (playerHP <= 30) {
                            hpText.setFillColor(sf::Color::Red);
                        } else if (playerHP <= 60) {
                            hpText.setFillColor(sf::Color::Yellow);
                        }

                        if (playerHP <= 0) {
                            gameState = GameState::GameOver;
                            std::cout << "Game Over! Final Score: " << score << "\n";
                        }
                        break;
                    }
                }
            }

            // 무적 깜빡임
            if (invincibilityTimer > 0) {
                int blink = static_cast<int>(invincibilityTimer * 10) % 2;
                player.setFillColor(blink ? sf::Color::Green : sf::Color(0, 255, 0, 100));
            } else {
                player.setFillColor(sf::Color::Green);
            }

            enemyText.setString("Enemies: " + std::to_string(enemies.size()) +
                               "/" + std::to_string(enemiesThisWave));

        } else if (gameState == GameState::WaveComplete) {
            waveCompleteTimer -= deltaTime;

            int countdown = static_cast<int>(waveCompleteTimer) + 1;
            nextWaveText.setString("Next wave in " + std::to_string(countdown) + "...");

            if (waveCompleteTimer <= 0) {
                // 다음 웨이브 시작
                currentWave++;
                enemiesThisWave = 5 + currentWave * 2;  // 웨이브마다 적 증가
                enemiesSpawned = 0;
                spawnInterval = std::max(0.5f, 1.5f - currentWave * 0.1f);  // 빨라짐
                gameState = GameState::Playing;

                waveText.setString("Wave " + std::to_string(currentWave));
                std::cout << "Wave " << currentWave << " Start! (" << enemiesThisWave << " enemies)\n";
            }
        }

        // 렌더링
        window.clear(sf::Color::Black);

        if (gameState == GameState::Playing || gameState == GameState::WaveComplete) {
            window.draw(player);
            window.draw(aimLine);

            for (const auto& bullet : bullets) {
                window.draw(bullet.shape);
            }

            for (const auto& enemy : enemies) {
                window.draw(enemy.shape);
            }

            window.draw(waveText);
            window.draw(scoreText);
            window.draw(hpText);
            window.draw(enemyText);

            if (gameState == GameState::WaveComplete) {
                // 반투명 오버레이
                sf::RectangleShape overlay({800, 600});
                overlay.setFillColor(sf::Color(0, 0, 0, 150));
                window.draw(overlay);
                window.draw(waveCompleteText);
                window.draw(nextWaveText);
            }

        } else if (gameState == GameState::GameOver) {
            window.draw(gameOverText);
            sf::Text finalScoreText(font, "Final Score: " + std::to_string(score), 32);
            finalScoreText.setPosition({250, 280});
            finalScoreText.setFillColor(sf::Color::Yellow);
            window.draw(finalScoreText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}
