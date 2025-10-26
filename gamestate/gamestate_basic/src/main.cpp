#include <SFML/Graphics.hpp>
#include <iostream>

// Game States Basic - SFML 3.0
//
// 게임 상태(State) 관리:
// - 메뉴, 게임플레이, 게임오버 등 화면 전환
// - enum으로 상태 정의
// - switch문으로 상태별 로직 분리
//
// 패턴:
// 1. enum으로 모든 상태 정의
// 2. 현재 상태를 변수로 저장
// 3. switch문으로 상태별 처리
//
// 게임잼에서 가장 많이 쓰는 패턴!

// 게임 상태 정의
enum class GameState {
    Menu,       // 메인 메뉴
    Playing,    // 게임 플레이 중
    GameOver    // 게임 오버
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game States Basic - SFML 3.0");
    window.setFramerateLimit(60);

    // 현재 상태
    GameState currentState = GameState::Menu;

    // 게임 데이터
    int score = 0;
    sf::Vector2f playerPos(400, 300);

    // UI 설정
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // 플레이어 (간단한 원)
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});

    // 텍스트
    sf::Text titleText(font, "GAME TITLE", 48);
    titleText.setPosition({250, 200});
    titleText.setFillColor(sf::Color::White);

    sf::Text instructionText(font, "Press SPACE to Start", 24);
    instructionText.setPosition({250, 300});
    instructionText.setFillColor(sf::Color::Yellow);

    sf::Text scoreText(font, "Score: 0", 20);
    scoreText.setPosition({10, 10});
    scoreText.setFillColor(sf::Color::White);

    sf::Text gameOverText(font, "GAME OVER", 48);
    gameOverText.setPosition({250, 200});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text finalScoreText(font, "Final Score: 0", 24);
    finalScoreText.setPosition({300, 300});
    finalScoreText.setFillColor(sf::Color::White);

    sf::Text restartText(font, "Press R to Restart", 20);
    restartText.setPosition({280, 400});
    restartText.setFillColor(sf::Color::Yellow);

    while (window.isOpen()) {
        // 이벤트 처리 - 상태별로 다르게!
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // 상태에 따라 다른 키 처리
                switch (currentState) {
                    case GameState::Menu:
                        if (keyPressed->code == sf::Keyboard::Key::Space) {
                            // 게임 시작!
                            currentState = GameState::Playing;
                            score = 0;
                            playerPos = {400, 300};
                            std::cout << "Game Started!\n";
                        }
                        break;

                    case GameState::Playing:
                        if (keyPressed->code == sf::Keyboard::Key::Escape) {
                            // 게임 오버로 (테스트용)
                            currentState = GameState::GameOver;
                            std::cout << "Game Over! Final Score: " << score << "\n";
                        }
                        break;

                    case GameState::GameOver:
                        if (keyPressed->code == sf::Keyboard::Key::R) {
                            // 메뉴로 돌아가기
                            currentState = GameState::Menu;
                            std::cout << "Back to Menu\n";
                        }
                        break;
                }
            }
        }

        // 업데이트 - 상태별로!
        switch (currentState) {
            case GameState::Menu:
                // 메뉴에서는 업데이트 없음
                break;

            case GameState::Playing: {
                // 플레이어 이동
                float speed = 200.0f / 60.0f;  // 간단하게 고정
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                    playerPos.y -= speed;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    playerPos.y += speed;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                    playerPos.x -= speed;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                    playerPos.x += speed;
                }

                // 점수 증가 (자동)
                score++;
                scoreText.setString("Score: " + std::to_string(score));

                // 플레이어 위치 업데이트
                player.setPosition(playerPos);
                break;
            }

            case GameState::GameOver:
                finalScoreText.setString("Final Score: " + std::to_string(score));
                break;
        }

        // 렌더링 - 상태별로!
        window.clear(sf::Color::Black);

        switch (currentState) {
            case GameState::Menu:
                window.draw(titleText);
                window.draw(instructionText);
                break;

            case GameState::Playing: {
                window.draw(player);
                window.draw(scoreText);
                // 안내 텍스트
                sf::Text hintText(font, "WASD: Move | ESC: Game Over", 14);
                hintText.setPosition({250, 570});
                hintText.setFillColor(sf::Color(150, 150, 150));
                window.draw(hintText);
                break;
            }

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
