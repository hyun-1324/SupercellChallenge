#include <SFML/Graphics.hpp>
#include <iostream>

// Game States - Pause - SFML 3.0
//
// 일시정지 기능:
// - 게임 중 ESC 키로 일시정지
// - 배경 게임 화면은 그대로 유지
// - 반투명 오버레이 + 일시정지 메뉴
// - Resume 또는 Quit 선택
//
// 게임잼 필수 기능!

enum class GameState {
    Menu,
    Playing,
    Paused,     // 일시정지 추가!
    GameOver
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game States - Pause - SFML 3.0");
    window.setFramerateLimit(60);

    GameState currentState = GameState::Menu;

    // 게임 데이터
    int score = 0;
    sf::Vector2f playerPos(400, 300);
    float playerSpeed = 200.0f;

    // UI
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});

    // Menu 화면
    sf::Text titleText(font, "PAUSE DEMO", 48);
    titleText.setPosition({250, 200});
    titleText.setFillColor(sf::Color::White);

    sf::Text startText(font, "Press SPACE to Start", 24);
    startText.setPosition({250, 300});
    startText.setFillColor(sf::Color::Yellow);

    // Playing 화면
    sf::Text scoreText(font, "Score: 0", 20);
    scoreText.setPosition({10, 10});
    scoreText.setFillColor(sf::Color::White);

    sf::Text hintText(font, "WASD: Move | ESC: Pause", 14);
    hintText.setPosition({250, 570});
    hintText.setFillColor(sf::Color(150, 150, 150));

    // Pause 화면 - 반투명 오버레이
    sf::RectangleShape pauseOverlay({800, 600});
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 180));  // 반투명 검정

    sf::Text pausedText(font, "PAUSED", 64);
    pausedText.setPosition({280, 150});
    pausedText.setFillColor(sf::Color::Yellow);

    sf::Text resumeText(font, "Press SPACE to Resume", 24);
    resumeText.setPosition({230, 280});
    resumeText.setFillColor(sf::Color::White);

    sf::Text quitText(font, "Press Q to Quit", 20);
    quitText.setPosition({280, 350});
    quitText.setFillColor(sf::Color(200, 200, 200));

    // GameOver 화면
    sf::Text gameOverText(font, "GAME OVER", 48);
    gameOverText.setPosition({250, 200});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text finalScoreText(font, "Final Score: 0", 24);
    finalScoreText.setPosition({300, 300});
    finalScoreText.setFillColor(sf::Color::White);

    sf::Text restartText(font, "Press R to Restart", 20);
    restartText.setPosition({280, 400});
    restartText.setFillColor(sf::Color::Yellow);

    sf::Clock deltaClock;

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
                            currentState = GameState::Playing;
                            score = 0;
                            playerPos = {400, 300};
                            std::cout << "Game Started!\n";
                        }
                        break;

                    case GameState::Playing:
                        if (keyPressed->code == sf::Keyboard::Key::Escape) {
                            // 일시정지!
                            currentState = GameState::Paused;
                            std::cout << "Game Paused\n";
                        }
                        // 테스트: G 키로 게임오버
                        if (keyPressed->code == sf::Keyboard::Key::G) {
                            currentState = GameState::GameOver;
                            std::cout << "Game Over!\n";
                        }
                        break;

                    case GameState::Paused:
                        if (keyPressed->code == sf::Keyboard::Key::Space) {
                            // 재개!
                            currentState = GameState::Playing;
                            std::cout << "Game Resumed\n";
                        }
                        if (keyPressed->code == sf::Keyboard::Key::Q) {
                            // 메뉴로
                            currentState = GameState::Menu;
                            std::cout << "Quit to Menu\n";
                        }
                        break;

                    case GameState::GameOver:
                        if (keyPressed->code == sf::Keyboard::Key::R) {
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                playerPos.y -= playerSpeed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                playerPos.y += playerSpeed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                playerPos.x -= playerSpeed * deltaTime;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                playerPos.x += playerSpeed * deltaTime;
            }

            // 화면 경계
            playerPos.x = std::max(20.f, std::min(780.f, playerPos.x));
            playerPos.y = std::max(20.f, std::min(580.f, playerPos.y));

            // 점수 증가
            score++;
            scoreText.setString("Score: " + std::to_string(score));

            player.setPosition(playerPos);
        }
        // Paused 상태에서는 업데이트 안 함! (게임 멈춤)

        if (currentState == GameState::GameOver) {
            finalScoreText.setString("Final Score: " + std::to_string(score));
        }

        // 렌더링
        window.clear(sf::Color::Black);

        switch (currentState) {
            case GameState::Menu:
                window.draw(titleText);
                window.draw(startText);
                break;

            case GameState::Playing:
                // 게임 화면
                window.draw(player);
                window.draw(scoreText);
                window.draw(hintText);
                break;

            case GameState::Paused:
                // 중요! 게임 화면을 먼저 그린 다음
                window.draw(player);
                window.draw(scoreText);
                window.draw(hintText);

                // 그 위에 오버레이 그리기
                window.draw(pauseOverlay);
                window.draw(pausedText);
                window.draw(resumeText);
                window.draw(quitText);
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
