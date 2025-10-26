#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Complete Game with Sound - SFML 3.0
//
// 게임 설명:
// - 코인을 수집하면서 적을 피하는 게임
// - 사운드: 배경음악, 코인 수집, 데미지, 게임오버
// - HP가 0이 되면 게임 오버
//
// 사운드 통합 패턴:
// 1. 게임 시작 시 배경음악 재생
// 2. 게임 이벤트마다 효과음 재생
// 3. 게임 오버 시 배경음악 정지, 게임오버 사운드 재생
// 4. 볼륨 설정 제공

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game with Sound - SFML 3.0");
    window.setFramerateLimit(60);

    // 배경음악
    sf::Music bgMusic;
    if (bgMusic.openFromFile("assets/bgmusic.wav") ||
        bgMusic.openFromFile("assets/bgmusic.ogg")) {
        bgMusic.setLooping(true);  // SFML 3.0
        bgMusic.setVolume(30);
        bgMusic.play();
    }

    // 효과음 버퍼
    sf::SoundBuffer coinBuffer, damageBuffer, gameoverBuffer;
    if (!coinBuffer.loadFromFile("assets/coin.wav")) {
        std::cout << "Warning: coin.wav not found\n";
    }
    if (!damageBuffer.loadFromFile("assets/click.wav")) {
        std::cout << "Warning: click.wav not found\n";
    }
    if (!gameoverBuffer.loadFromFile("assets/jump.wav")) {
        std::cout << "Warning: jump.wav not found\n";
    }

    // 효과음
    sf::Sound coinSound(coinBuffer);
    coinSound.setVolume(50);

    sf::Sound damageSound(damageBuffer);
    damageSound.setVolume(70);

    sf::Sound gameoverSound(gameoverBuffer);
    gameoverSound.setVolume(80);

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setPosition({400, 300});

    // 코인들
    std::vector<sf::CircleShape> coins;
    for (int i = 0; i < 10; i++) {
        sf::CircleShape coin(10);
        coin.setFillColor(sf::Color::Yellow);
        coin.setPosition({
            static_cast<float>(std::rand() % 760 + 20),
            static_cast<float>(std::rand() % 560 + 20)
        });
        coins.push_back(coin);
    }

    // 적들
    std::vector<sf::CircleShape> enemies;
    for (int i = 0; i < 5; i++) {
        sf::CircleShape enemy(15);
        enemy.setFillColor(sf::Color::Red);
        enemy.setPosition({
            static_cast<float>(std::rand() % 760 + 20),
            static_cast<float>(std::rand() % 560 + 20)
        });
        enemies.push_back(enemy);
    }

    // 게임 상태
    int score = 0;
    int hp = 100;
    bool gameOver = false;
    float invincibilityTimer = 0.0f;  // 무적 시간

    // UI
    sf::Font font;
    if (!font.openFromFile("assets/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text scoreText(font, "Score: 0", 20);
    scoreText.setPosition({10, 10});

    sf::Text hpText(font, "HP: 100", 20);
    hpText.setPosition({10, 40});
    hpText.setFillColor(sf::Color::Green);

    sf::Text gameOverText(font, "GAME OVER\nPress R to Restart", 40);
    gameOverText.setPosition({250, 250});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text instructionText(font, "WASD: Move  |  Collect Yellow Coins  |  Avoid Red Enemies", 14);
    instructionText.setPosition({150, 570});

    sf::Text volumeText(font, "Volume: Up/Down keys", 12);
    volumeText.setPosition({650, 10});

    sf::Clock deltaClock;
    sf::Clock invincibilityClock;

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();

        // 이벤트 처리
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // 재시작
                if (keyPressed->code == sf::Keyboard::Key::R && gameOver) {
                    // 게임 리셋
                    score = 0;
                    hp = 100;
                    gameOver = false;
                    player.setPosition({400, 300});

                    // 코인 재생성
                    coins.clear();
                    for (int i = 0; i < 10; i++) {
                        sf::CircleShape coin(10);
                        coin.setFillColor(sf::Color::Yellow);
                        coin.setPosition({
                            static_cast<float>(std::rand() % 760 + 20),
                            static_cast<float>(std::rand() % 560 + 20)
                        });
                        coins.push_back(coin);
                    }

                    // 적 재생성
                    enemies.clear();
                    for (int i = 0; i < 5; i++) {
                        sf::CircleShape enemy(15);
                        enemy.setFillColor(sf::Color::Red);
                        enemy.setPosition({
                            static_cast<float>(std::rand() % 760 + 20),
                            static_cast<float>(std::rand() % 560 + 20)
                        });
                        enemies.push_back(enemy);
                    }

                    // 배경음악 재시작
                    bgMusic.play();
                }

                // 볼륨 조절
                if (keyPressed->code == sf::Keyboard::Key::Up) {
                    float volume = bgMusic.getVolume();
                    bgMusic.setVolume(std::min(100.f, volume + 10));
                }
                if (keyPressed->code == sf::Keyboard::Key::Down) {
                    float volume = bgMusic.getVolume();
                    bgMusic.setVolume(std::max(0.f, volume - 10));
                }
            }
        }

        if (!gameOver) {
            // 플레이어 이동
            float speed = 200.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                player.move({0, -speed * deltaTime});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                player.move({0, speed * deltaTime});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                player.move({-speed * deltaTime, 0});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                player.move({speed * deltaTime, 0});
            }

            // 화면 경계 체크
            sf::Vector2f playerPos = player.getPosition();
            if (playerPos.x < 0) player.setPosition({0, playerPos.y});
            if (playerPos.x > 760) player.setPosition({760, playerPos.y});
            if (playerPos.y < 0) player.setPosition({playerPos.x, 0});
            if (playerPos.y > 560) player.setPosition({playerPos.x, 560});

            // 무적 타이머 감소
            if (invincibilityTimer > 0) {
                invincibilityTimer -= deltaTime;
            }

            // 코인 충돌 체크
            auto playerBounds = player.getGlobalBounds();
            for (auto it = coins.begin(); it != coins.end(); ) {
                if (playerBounds.findIntersection(it->getGlobalBounds()).has_value()) {
                    score += 10;
                    scoreText.setString("Score: " + std::to_string(score));
                    coinSound.play();  // 코인 수집 소리
                    it = coins.erase(it);

                    // 새 코인 생성
                    sf::CircleShape newCoin(10);
                    newCoin.setFillColor(sf::Color::Yellow);
                    newCoin.setPosition({
                        static_cast<float>(std::rand() % 760 + 20),
                        static_cast<float>(std::rand() % 560 + 20)
                    });
                    coins.push_back(newCoin);
                } else {
                    ++it;
                }
            }

            // 적 충돌 체크
            if (invincibilityTimer <= 0) {
                for (const auto& enemy : enemies) {
                    if (playerBounds.findIntersection(enemy.getGlobalBounds()).has_value()) {
                        hp -= 20;
                        hpText.setString("HP: " + std::to_string(hp));
                        damageSound.play();  // 데미지 소리
                        invincibilityTimer = 1.0f;  // 1초 무적

                        // HP 색상 변경
                        if (hp > 50) {
                            hpText.setFillColor(sf::Color::Green);
                        } else if (hp > 20) {
                            hpText.setFillColor(sf::Color::Yellow);
                        } else {
                            hpText.setFillColor(sf::Color::Red);
                        }

                        if (hp <= 0) {
                            gameOver = true;
                            bgMusic.stop();           // 배경음악 정지
                            gameoverSound.play();     // 게임오버 소리
                        }
                        break;
                    }
                }
            }

            // 적 이동 (플레이어를 향해)
            for (auto& enemy : enemies) {
                sf::Vector2f enemyPos = enemy.getPosition();
                sf::Vector2f direction = player.getPosition() - enemyPos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length > 0) {
                    direction /= length;  // 정규화
                    enemy.move(direction * 50.0f * deltaTime);
                }
            }

            // 무적 시간 동안 플레이어 깜빡임
            if (invincibilityTimer > 0) {
                int blink = static_cast<int>(invincibilityTimer * 10) % 2;
                player.setFillColor(blink ? sf::Color::Green : sf::Color(0, 255, 0, 100));
            } else {
                player.setFillColor(sf::Color::Green);
            }
        }

        // 렌더링
        window.clear(sf::Color::Black);

        // 게임 오브젝트
        window.draw(player);
        for (const auto& coin : coins) {
            window.draw(coin);
        }
        for (const auto& enemy : enemies) {
            window.draw(enemy);
        }

        // UI
        window.draw(scoreText);
        window.draw(hpText);
        window.draw(instructionText);
        window.draw(volumeText);

        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
