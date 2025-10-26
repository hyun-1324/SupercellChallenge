#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Game States Complete - SFML 3.0
//
// 완전한 게임 예제:
// - Menu → Playing → Pause → GameOver
// - 실제 게임 메커니즘 (코인 수집, 적 피하기)
// - 모든 상태 전환 구현
// - 게임잼에 바로 쓸 수 있는 템플릿!

enum class GameState {
    Menu,
    Playing,
    Paused,
    GameOver
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Complete Game - SFML 3.0");
    window.setFramerateLimit(60);

    GameState currentState = GameState::Menu;

    // 게임 데이터
    int score = 0;
    int lives = 3;
    float invincibilityTimer = 0.0f;

    // 플레이어
    sf::CircleShape player(15);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({15, 15});
    player.setPosition({400, 500});
    float playerSpeed = 250.0f;

    // 코인들
    std::vector<sf::CircleShape> coins;
    float coinSpawnTimer = 0.0f;
    const float coinSpawnInterval = 1.5f;

    // 적들
    std::vector<sf::CircleShape> enemies;
    float enemySpawnTimer = 0.0f;
    const float enemySpawnInterval = 2.0f;

    // UI
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // === MENU 화면 ===
    sf::Text titleText(font, "COIN COLLECTOR", 56);
    titleText.setPosition({180, 150});
    titleText.setFillColor(sf::Color::Yellow);

    sf::Text menuStartText(font, "Press SPACE to Start", 28);
    menuStartText.setPosition({220, 300});
    menuStartText.setFillColor(sf::Color::White);

    sf::Text menuInfoText(font, "Collect coins, avoid enemies!", 18);
    menuInfoText.setPosition({240, 380});
    menuInfoText.setFillColor(sf::Color(150, 150, 150));

    // === PLAYING 화면 ===
    sf::Text scoreText(font, "Score: 0", 20);
    scoreText.setPosition({10, 10});
    scoreText.setFillColor(sf::Color::White);

    sf::Text livesText(font, "Lives: 3", 20);
    livesText.setPosition({10, 40});
    livesText.setFillColor(sf::Color::Green);

    sf::Text controlsText(font, "Arrow Keys: Move | ESC: Pause", 14);
    controlsText.setPosition({250, 575});
    controlsText.setFillColor(sf::Color(150, 150, 150));

    // === PAUSE 화면 ===
    sf::RectangleShape pauseOverlay({800, 600});
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 200));

    sf::Text pausedText(font, "PAUSED", 72);
    pausedText.setPosition({260, 180});
    pausedText.setFillColor(sf::Color::Yellow);

    sf::Text pauseResumeText(font, "SPACE: Resume", 24);
    pauseResumeText.setPosition({280, 320});
    pauseResumeText.setFillColor(sf::Color::White);

    sf::Text pauseQuitText(font, "Q: Quit to Menu", 24);
    pauseQuitText.setPosition({280, 370});
    pauseQuitText.setFillColor(sf::Color(200, 200, 200));

    // === GAMEOVER 화면 ===
    sf::Text gameOverText(font, "GAME OVER", 64);
    gameOverText.setPosition({220, 180});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text finalScoreText(font, "Final Score: 0", 32);
    finalScoreText.setPosition({270, 290});
    finalScoreText.setFillColor(sf::Color::Yellow);

    sf::Text restartText(font, "R: Restart | Q: Quit", 24);
    restartText.setPosition({250, 380});
    restartText.setFillColor(sf::Color::White);

    sf::Clock deltaClock;

    // 게임 초기화 함수
    auto resetGame = [&]() {
        score = 0;
        lives = 3;
        invincibilityTimer = 0.0f;
        player.setPosition({400, 500});
        coins.clear();
        enemies.clear();
        coinSpawnTimer = 0.0f;
        enemySpawnTimer = 0.0f;
        scoreText.setString("Score: 0");
        livesText.setString("Lives: 3");
        livesText.setFillColor(sf::Color::Green);
    };

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();

        // 이벤트 처리
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (currentState) {
                    case GameState::Menu:
                        if (keyPressed->code == sf::Keyboard::Key::Space) {
                            resetGame();
                            currentState = GameState::Playing;
                            std::cout << "Game Started!\n";
                        }
                        break;

                    case GameState::Playing:
                        if (keyPressed->code == sf::Keyboard::Key::Escape) {
                            currentState = GameState::Paused;
                            std::cout << "Game Paused\n";
                        }
                        break;

                    case GameState::Paused:
                        if (keyPressed->code == sf::Keyboard::Key::Space) {
                            currentState = GameState::Playing;
                            std::cout << "Game Resumed\n";
                        }
                        if (keyPressed->code == sf::Keyboard::Key::Q) {
                            currentState = GameState::Menu;
                            std::cout << "Quit to Menu\n";
                        }
                        break;

                    case GameState::GameOver:
                        if (keyPressed->code == sf::Keyboard::Key::R) {
                            resetGame();
                            currentState = GameState::Playing;
                            std::cout << "Game Restarted!\n";
                        }
                        if (keyPressed->code == sf::Keyboard::Key::Q) {
                            currentState = GameState::Menu;
                            std::cout << "Back to Menu\n";
                        }
                        break;
                }
            }
        }

        // 업데이트 - Playing일 때만!
        if (currentState == GameState::Playing) {
            // 플레이어 이동
            sf::Vector2f movement(0, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                movement.x -= playerSpeed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                movement.x += playerSpeed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                movement.y -= playerSpeed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                movement.y += playerSpeed * deltaTime;
            }

            sf::Vector2f newPos = player.getPosition() + movement;
            newPos.x = std::max(15.f, std::min(785.f, newPos.x));
            newPos.y = std::max(15.f, std::min(585.f, newPos.y));
            player.setPosition(newPos);

            // 무적 타이머
            if (invincibilityTimer > 0) {
                invincibilityTimer -= deltaTime;
            }

            // 코인 생성
            coinSpawnTimer += deltaTime;
            if (coinSpawnTimer >= coinSpawnInterval) {
                coinSpawnTimer = 0;
                sf::CircleShape coin(8);
                coin.setFillColor(sf::Color::Yellow);
                coin.setOrigin({8, 8});
                coin.setPosition({
                    static_cast<float>(std::rand() % 760 + 20),
                    static_cast<float>(std::rand() % 400 + 20)
                });
                coins.push_back(coin);
            }

            // 적 생성
            enemySpawnTimer += deltaTime;
            if (enemySpawnTimer >= enemySpawnInterval) {
                enemySpawnTimer = 0;
                sf::CircleShape enemy(12);
                enemy.setFillColor(sf::Color::Red);
                enemy.setOrigin({12, 12});
                enemy.setPosition({
                    static_cast<float>(std::rand() % 760 + 20),
                    -20.0f  // 위에서 떨어짐
                });
                enemies.push_back(enemy);
            }

            // 적 이동
            for (auto& enemy : enemies) {
                enemy.move({0, 100.0f * deltaTime});  // 아래로
            }

            // 화면 밖 적 제거
            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [](const sf::CircleShape& e) {
                        return e.getPosition().y > 620;
                    }),
                enemies.end()
            );

            // 코인 수집
            auto playerBounds = player.getGlobalBounds();
            for (auto it = coins.begin(); it != coins.end(); ) {
                if (playerBounds.findIntersection(it->getGlobalBounds()).has_value()) {
                    score += 10;
                    scoreText.setString("Score: " + std::to_string(score));
                    it = coins.erase(it);
                } else {
                    ++it;
                }
            }

            // 적 충돌
            if (invincibilityTimer <= 0) {
                for (const auto& enemy : enemies) {
                    if (playerBounds.findIntersection(enemy.getGlobalBounds()).has_value()) {
                        lives--;
                        invincibilityTimer = 2.0f;
                        livesText.setString("Lives: " + std::to_string(lives));

                        if (lives <= 1) {
                            livesText.setFillColor(sf::Color::Red);
                        } else if (lives <= 2) {
                            livesText.setFillColor(sf::Color::Yellow);
                        }

                        if (lives <= 0) {
                            currentState = GameState::GameOver;
                            finalScoreText.setString("Final Score: " + std::to_string(score));
                            std::cout << "Game Over! Score: " << score << "\n";
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
        }

        // 렌더링
        window.clear(sf::Color::Black);

        switch (currentState) {
            case GameState::Menu:
                window.draw(titleText);
                window.draw(menuStartText);
                window.draw(menuInfoText);
                break;

            case GameState::Playing:
                // 게임 오브젝트
                for (const auto& coin : coins) {
                    window.draw(coin);
                }
                for (const auto& enemy : enemies) {
                    window.draw(enemy);
                }
                window.draw(player);

                // UI
                window.draw(scoreText);
                window.draw(livesText);
                window.draw(controlsText);
                break;

            case GameState::Paused:
                // 게임 화면 먼저
                for (const auto& coin : coins) {
                    window.draw(coin);
                }
                for (const auto& enemy : enemies) {
                    window.draw(enemy);
                }
                window.draw(player);
                window.draw(scoreText);
                window.draw(livesText);

                // 오버레이
                window.draw(pauseOverlay);
                window.draw(pausedText);
                window.draw(pauseResumeText);
                window.draw(pauseQuitText);
                break;

            case GameState::GameOver:
                window.draw(gameOverText);
                window.draw(finalScoreText);
                window.draw(restartText);
                break;
        }

        window.display();
    }

    return 0;
}
