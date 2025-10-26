#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Text - Game UI");
    window.setFramerateLimit(60);

    // 폰트 로드
    sf::Font font;
    if (!font.openFromFile("../assets/Arial.ttf")) {
        if (!font.openFromFile("../assets/font.ttc")) {
            std::cout << "폰트 로드 실패!" << std::endl;
            return -1;
        }
    }

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});
    player.setPosition({400, 300});

    // 적들
    std::vector<sf::CircleShape> enemies;
    for (int i = 0; i < 3; ++i) {
        sf::CircleShape enemy(15);
        enemy.setFillColor(sf::Color::Red);
        enemy.setOrigin({15, 15});
        enemy.setPosition({200.f + i * 200.f, 100.f});
        enemies.push_back(enemy);
    }

    // 게임 변수
    int hp = 100;
    int maxHp = 100;
    int score = 0;
    float gameTime = 0.0f;

    float speed = 250.0f;

    sf::Clock clock;
    sf::Clock gameClock;

    // === UI 텍스트들 ===

    // HP 바
    sf::Text hpLabel(font);
    hpLabel.setString("HP:");
    hpLabel.setCharacterSize(24);
    hpLabel.setFillColor(sf::Color::White);
    hpLabel.setPosition({20, 20});

    sf::Text hpText(font);
    hpText.setCharacterSize(24);
    hpText.setFillColor(sf::Color::Green);
    hpText.setPosition({80, 20});

    // HP 바 (그래픽)
    sf::RectangleShape hpBarBg({200, 20});
    hpBarBg.setFillColor(sf::Color(50, 50, 50));
    hpBarBg.setPosition({20, 55});

    sf::RectangleShape hpBarFill({200, 20});
    hpBarFill.setFillColor(sf::Color::Green);
    hpBarFill.setPosition({20, 55});

    // 점수
    sf::Text scoreLabel(font);
    scoreLabel.setString("Score:");
    scoreLabel.setCharacterSize(28);
    scoreLabel.setFillColor(sf::Color::Yellow);
    scoreLabel.setOutlineColor(sf::Color::Black);
    scoreLabel.setOutlineThickness(2);
    scoreLabel.setPosition({600, 20});

    sf::Text scoreText(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition({720, 20});

    // 타이머
    sf::Text timerText(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Cyan);
    timerText.setPosition({350, 20});

    // 게임 오버
    sf::Text gameOverText(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(3);
    gameOverText.setStyle(sf::Text::Bold);

    sf::FloatRect goTextBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(goTextBounds.size / 2.f);
    gameOverText.setPosition({400, 300});

    bool gameOver = false;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // R키로 재시작
            if (gameOver) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::R) {
                        // 재시작
                        hp = maxHp;
                        score = 0;
                        gameOver = false;
                        player.setPosition({400, 300});
                        gameClock.restart();
                    }
                }
            }
        }

        if (!gameOver) {
            // 게임 시간 업데이트
            gameTime = gameClock.getElapsedTime().asSeconds();

            // 플레이어 이동
            sf::Vector2f movement(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) movement.x -= 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) movement.x += 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) movement.y -= 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) movement.y += 1.0f;

            player.move(movement * speed * deltaTime);

            // 화면 제한
            sf::Vector2f pos = player.getPosition();
            if (pos.x < 20) pos.x = 20;
            if (pos.x > 780) pos.x = 780;
            if (pos.y < 100) pos.y = 100;
            if (pos.y > 580) pos.y = 580;
            player.setPosition(pos);

            // 적과 충돌 (간단한 예시)
            sf::FloatRect playerBounds = player.getGlobalBounds();
            for (const auto& enemy : enemies) {
                if (playerBounds.findIntersection(enemy.getGlobalBounds()).has_value()) {
                    hp -= 1;  // 프레임당 1씩 감소 (터치하면 빠르게 감소)
                    if (hp < 0) hp = 0;
                }
            }

            // 점수 증가 (시간에 따라)
            score = static_cast<int>(gameTime * 10);

            // HP에 따라 색상 변경
            if (hp > 50) {
                hpText.setFillColor(sf::Color::Green);
                hpBarFill.setFillColor(sf::Color::Green);
            } else if (hp > 20) {
                hpText.setFillColor(sf::Color::Yellow);
                hpBarFill.setFillColor(sf::Color::Yellow);
            } else {
                hpText.setFillColor(sf::Color::Red);
                hpBarFill.setFillColor(sf::Color::Red);
            }

            // UI 업데이트
            hpText.setString(std::to_string(hp) + " / " + std::to_string(maxHp));

            float hpPercent = static_cast<float>(hp) / maxHp;
            hpBarFill.setSize({200 * hpPercent, 20});

            scoreText.setString(std::to_string(score));

            int minutes = static_cast<int>(gameTime) / 60;
            int seconds = static_cast<int>(gameTime) % 60;
            std::stringstream ss;
            ss << "Time: " << minutes << ":";
            if (seconds < 10) ss << "0";
            ss << seconds;
            timerText.setString(ss.str());

            // 게임 오버 체크
            if (hp <= 0) {
                gameOver = true;
            }
        }

        window.clear(sf::Color(30, 30, 50));  // 어두운 파란색 배경

        // 게임 객체들
        for (const auto& enemy : enemies) {
            window.draw(enemy);
        }
        window.draw(player);

        // UI 그리기
        window.draw(hpLabel);
        window.draw(hpText);
        window.draw(hpBarBg);
        window.draw(hpBarFill);
        window.draw(scoreLabel);
        window.draw(scoreText);
        window.draw(timerText);

        if (gameOver) {
            // 반투명 오버레이
            sf::RectangleShape overlay({800, 600});
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            window.draw(gameOverText);

            // 재시작 안내
            sf::Text restartText(font);
            restartText.setString("Press R to Restart");
            restartText.setCharacterSize(32);
            restartText.setFillColor(sf::Color::White);
            sf::FloatRect restartBounds = restartText.getLocalBounds();
            restartText.setOrigin(restartBounds.size / 2.f);
            restartText.setPosition({400, 380});
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}

/*
실전 게임 UI 핵심:

1. HP 시스템:
   - 숫자 표시: "100 / 100"
   - HP 바 그래픽
   - 색상으로 상태 표시 (초록/노랑/빨강)

2. 점수 시스템:
   - 동적 업데이트
   - 외곽선으로 가독성

3. 타이머:
   - MM:SS 형식
   - 게임 진행 시간 표시

4. 게임 오버:
   - 반투명 오버레이
   - 큰 텍스트로 강조
   - 재시작 기능

5. 게임잼 팁:
   - UI는 항상 위에 그리기
   - 외곽선으로 가독성 확보
   - 색상으로 정보 전달
   - 간단하고 명확하게!
*/
