#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Top-Down Shooter Complete - SFML 3.0
//
// 완전한 슈터 게임:
// - 웨이브 시스템
// - 파워업 (체력 회복, 연사력, 데미지)
// - 업그레이드 시스템
// - 게임 상태 관리
//
// 게임잼 템플릿!

struct Bullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    int damage;

    Bullet(sf::Vector2f pos, sf::Vector2f vel, int dmg = 1) {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin({5, 5});
        shape.setPosition(pos);
        velocity = vel;
        damage = dmg;
    }
};

struct Enemy {
    sf::CircleShape shape;
    int hp;
    int maxHp;
    float speed;

    Enemy(sf::Vector2f pos, int wave) {
        shape.setRadius(15);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin({15, 15});
        shape.setPosition(pos);
        maxHp = 2 + wave;
        hp = maxHp;
        speed = 80.0f + wave * 10.0f;
    }
};

enum class PowerUpType {
    Health,     // 체력 회복
    FireRate,   // 연사력 증가
    Damage      // 데미지 증가
};

struct PowerUp {
    sf::CircleShape shape;
    PowerUpType type;

    PowerUp(sf::Vector2f pos, PowerUpType t) {
        shape.setRadius(12);
        shape.setOrigin({12, 12});
        shape.setPosition(pos);
        type = t;

        switch (type) {
            case PowerUpType::Health:
                shape.setFillColor(sf::Color::Green);
                break;
            case PowerUpType::FireRate:
                shape.setFillColor(sf::Color::Cyan);
                break;
            case PowerUpType::Damage:
                shape.setFillColor(sf::Color::Magenta);
                break;
        }
    }
};

enum class GameState {
    Menu,
    Playing,
    WaveComplete,
    GameOver
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Shooter Complete - SFML 3.0");
    window.setFramerateLimit(60);

    GameState gameState = GameState::Menu;

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});
    player.setPosition({400, 300});
    float playerSpeed = 250.0f;
    int playerHP = 100;
    int maxHP = 100;

    // 업그레이드
    int damageLevel = 1;
    float fireRateLevel = 1.0f;
    float shootInterval = 0.2f;

    sf::RectangleShape aimLine({30, 2});
    aimLine.setFillColor(sf::Color::White);
    aimLine.setOrigin({0, 1});

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<PowerUp> powerUps;

    // 웨이브
    int currentWave = 1;
    int enemiesThisWave = 5;
    int enemiesSpawned = 0;
    float spawnTimer = 0.0f;
    float spawnInterval = 1.5f;
    float waveCompleteTimer = 0.0f;

    int score = 0;
    float shootCooldown = 0.0f;
    float invincibilityTimer = 0.0f;
    float powerUpSpawnTimer = 0.0f;

    // UI
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // Menu
    sf::Text titleText(font, "TOP-DOWN SHOOTER", 48);
    titleText.setPosition({180, 180});
    titleText.setFillColor(sf::Color::Yellow);

    sf::Text startText(font, "Press SPACE to Start", 24);
    startText.setPosition({240, 300});
    startText.setFillColor(sf::Color::White);

    // Playing UI
    sf::Text waveText(font, "Wave 1", 28);
    waveText.setPosition({10, 10});
    waveText.setFillColor(sf::Color::Yellow);

    sf::Text scoreText(font, "Score: 0", 18);
    scoreText.setPosition({10, 45});

    sf::Text hpText(font, "HP: 100/100", 18);
    hpText.setPosition({10, 70});
    hpText.setFillColor(sf::Color::Green);

    sf::Text upgradeText(font, "", 16);
    upgradeText.setPosition({10, 95});
    upgradeText.setFillColor(sf::Color::Cyan);

    // Wave Complete
    sf::Text waveCompleteText(font, "WAVE COMPLETE!", 48);
    waveCompleteText.setPosition({200, 230});
    waveCompleteText.setFillColor(sf::Color::Yellow);

    sf::Text nextWaveText(font, "Next wave in 3...", 24);
    nextWaveText.setPosition({280, 300});

    // Game Over
    sf::Text gameOverText(font, "GAME OVER", 56);
    gameOverText.setPosition({220, 200});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text finalScoreText(font, "", 28);
    finalScoreText.setPosition({280, 280});
    finalScoreText.setFillColor(sf::Color::Yellow);

    sf::Text restartText(font, "Press R to Restart", 20);
    restartText.setPosition({280, 340});

    sf::Clock deltaClock;

    auto resetGame = [&]() {
        gameState = GameState::Playing;
        currentWave = 1;
        enemiesThisWave = 5;
        enemiesSpawned = 0;
        score = 0;
        playerHP = 100;
        maxHP = 100;
        damageLevel = 1;
        fireRateLevel = 1.0f;
        shootInterval = 0.2f;
        player.setPosition({400, 300});
        bullets.clear();
        enemies.clear();
        powerUps.clear();
        waveText.setString("Wave 1");
        scoreText.setString("Score: 0");
        hpText.setString("HP: 100/100");
        hpText.setFillColor(sf::Color::Green);
    };

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space && gameState == GameState::Menu) {
                    resetGame();
                }
                if (keyPressed->code == sf::Keyboard::Key::R && gameState == GameState::GameOver) {
                    gameState = GameState::Menu;
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left &&
                    shootCooldown <= 0 && gameState == GameState::Playing) {

                    sf::Vector2f direction = mousePos - player.getPosition();
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                    if (length > 0) {
                        direction /= length;
                        bullets.emplace_back(player.getPosition(), direction * 500.0f, damageLevel);
                        shootCooldown = shootInterval / fireRateLevel;
                    }
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

            // 적 생성
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

            // 파워업 생성
            powerUpSpawnTimer += deltaTime;
            if (powerUpSpawnTimer >= 10.0f) {  // 10초마다
                powerUpSpawnTimer = 0;
                PowerUpType type = static_cast<PowerUpType>(std::rand() % 3);
                powerUps.emplace_back(
                    sf::Vector2f(
                        static_cast<float>(std::rand() % 700 + 50),
                        static_cast<float>(std::rand() % 500 + 50)
                    ),
                    type
                );
            }

            // 웨이브 완료
            if (enemiesSpawned >= enemiesThisWave && enemies.empty()) {
                gameState = GameState::WaveComplete;
                waveCompleteTimer = 3.0f;
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

                        enemyIt->hp -= bulletIt->damage;

                        if (enemyIt->hp <= 0) {
                            score += 10 * currentWave;
                            scoreText.setString("Score: " + std::to_string(score));
                            enemyIt = enemies.erase(enemyIt);
                        } else {
                            float hpPercent = static_cast<float>(enemyIt->hp) / enemyIt->maxHp;
                            std::uint8_t red = 255;
                            std::uint8_t green = static_cast<std::uint8_t>(hpPercent * 150);
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

            // 파워업 수집
            auto playerBounds = player.getGlobalBounds();
            for (auto it = powerUps.begin(); it != powerUps.end(); ) {
                if (playerBounds.findIntersection(it->shape.getGlobalBounds()).has_value()) {
                    switch (it->type) {
                        case PowerUpType::Health:
                            playerHP = std::min(maxHP, playerHP + 30);
                            std::cout << "Health +30!\n";
                            break;
                        case PowerUpType::FireRate:
                            fireRateLevel += 0.3f;
                            std::cout << "Fire Rate Up!\n";
                            break;
                        case PowerUpType::Damage:
                            damageLevel++;
                            std::cout << "Damage Up!\n";
                            break;
                    }
                    it = powerUps.erase(it);
                } else {
                    ++it;
                }
            }

            // 적 vs 플레이어
            if (invincibilityTimer <= 0) {
                for (const auto& enemy : enemies) {
                    if (playerBounds.findIntersection(enemy.shape.getGlobalBounds()).has_value()) {
                        playerHP -= 10;
                        invincibilityTimer = 1.0f;

                        if (playerHP <= 0) {
                            gameState = GameState::GameOver;
                            finalScoreText.setString("Score: " + std::to_string(score));
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

            // UI 업데이트
            hpText.setString("HP: " + std::to_string(playerHP) + "/" + std::to_string(maxHP));
            if (playerHP <= 30) {
                hpText.setFillColor(sf::Color::Red);
            } else if (playerHP <= 60) {
                hpText.setFillColor(sf::Color::Yellow);
            } else {
                hpText.setFillColor(sf::Color::Green);
            }

            upgradeText.setString("DMG:" + std::to_string(damageLevel) +
                                 " FR:" + std::to_string(fireRateLevel).substr(0, 3) + "x");

        } else if (gameState == GameState::WaveComplete) {
            waveCompleteTimer -= deltaTime;
            int countdown = static_cast<int>(waveCompleteTimer) + 1;
            nextWaveText.setString("Next wave in " + std::to_string(countdown) + "...");

            if (waveCompleteTimer <= 0) {
                currentWave++;
                enemiesThisWave = 5 + currentWave * 2;
                enemiesSpawned = 0;
                spawnInterval = std::max(0.5f, 1.5f - currentWave * 0.1f);
                gameState = GameState::Playing;
                waveText.setString("Wave " + std::to_string(currentWave));
            }
        }

        // 렌더링
        window.clear(sf::Color::Black);

        if (gameState == GameState::Menu) {
            window.draw(titleText);
            window.draw(startText);

        } else if (gameState == GameState::Playing || gameState == GameState::WaveComplete) {
            window.draw(player);
            window.draw(aimLine);

            for (const auto& bullet : bullets) {
                window.draw(bullet.shape);
            }

            for (const auto& enemy : enemies) {
                window.draw(enemy.shape);
            }

            for (const auto& powerUp : powerUps) {
                window.draw(powerUp.shape);
            }

            window.draw(waveText);
            window.draw(scoreText);
            window.draw(hpText);
            window.draw(upgradeText);

            if (gameState == GameState::WaveComplete) {
                sf::RectangleShape overlay({800, 600});
                overlay.setFillColor(sf::Color(0, 0, 0, 150));
                window.draw(overlay);
                window.draw(waveCompleteText);
                window.draw(nextWaveText);
            }

        } else if (gameState == GameState::GameOver) {
            window.draw(gameOverText);
            window.draw(finalScoreText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}
