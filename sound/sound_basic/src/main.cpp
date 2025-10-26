#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Sound & Music Basic - SFML 3.0
//
// 개념:
// - sf::SoundBuffer: 사운드 데이터를 메모리에 로드 (짧은 효과음용)
// - sf::Sound: 사운드 재생
// - sf::Music: 긴 음악 파일 스트리밍 재생
//
// Sound vs Music:
// - Sound: 짧은 효과음 (충돌, 점프, 클릭) - 전체를 메모리에 로드
// - Music: 긴 배경음악 - 스트리밍으로 재생 (메모리 효율적)
//
// 사운드 파일 준비:
// 1. assets/ 폴더에 click.wav, jump.wav, coin.wav 추가
// 2. 무료 사운드: freesound.org, opengameart.org
// 3. 또는 온라인 beep generator 사용

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Sound Basic - SFML 3.0");
    window.setFramerateLimit(60);

    // 사운드 버퍼 로드 (메모리에 사운드 데이터 저장)
    sf::SoundBuffer clickBuffer;
    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer coinBuffer;

    // 사운드 파일 로드 - SFML 3.0에서는 loadFromFile 사용
    if (!clickBuffer.loadFromFile("assets/click.wav")) {
        std::cout << "Warning: click.wav not found. Add sound files to assets/ folder\n";
        std::cout << "Free sounds: freesound.org or opengameart.org\n";
    }
    if (!jumpBuffer.loadFromFile("assets/jump.wav")) {
        std::cout << "Warning: jump.wav not found\n";
    }
    if (!coinBuffer.loadFromFile("assets/coin.wav")) {
        std::cout << "Warning: coin.wav not found\n";
    }

    // Sound 객체 생성 (SoundBuffer를 사용)
    sf::Sound clickSound(clickBuffer);
    sf::Sound jumpSound(jumpBuffer);
    sf::Sound coinSound(coinBuffer);

    // UI 설정
    sf::Font font;
    if (!font.openFromFile("assets/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // 버튼 생성
    sf::RectangleShape clickButton({200, 60});
    clickButton.setPosition({100, 200});
    clickButton.setFillColor(sf::Color::Blue);

    sf::RectangleShape jumpButton({200, 60});
    jumpButton.setPosition({100, 300});
    jumpButton.setFillColor(sf::Color::Green);

    sf::RectangleShape coinButton({200, 60});
    coinButton.setPosition({100, 400});
    coinButton.setFillColor(sf::Color::Yellow);

    // 텍스트
    sf::Text titleText(font, "Click Buttons to Play Sounds", 24);
    titleText.setPosition({200, 50});
    titleText.setFillColor(sf::Color::Black);

    sf::Text clickText(font, "Click Sound", 20);
    clickText.setPosition({130, 215});
    clickText.setFillColor(sf::Color::White);

    sf::Text jumpText(font, "Jump Sound", 20);
    jumpText.setPosition({130, 315});
    jumpText.setFillColor(sf::Color::White);

    sf::Text coinText(font, "Coin Sound", 20);
    coinText.setPosition({130, 415});
    coinText.setFillColor(sf::Color::Black);

    sf::Text infoText(font, "Sound Status: Ready", 16);
    infoText.setPosition({400, 200});
    infoText.setFillColor(sf::Color::Black);

    sf::Text instructionText(font, "Space: Play Click\nJ: Play Jump\nC: Play Coin", 14);
    instructionText.setPosition({400, 300});
    instructionText.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        // 이벤트 처리 - SFML 3.0 스타일
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // 마우스 클릭
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                                     static_cast<float>(mousePressed->position.y));

                if (clickButton.getGlobalBounds().contains(mousePos)) {
                    clickSound.play();  // 사운드 재생
                    infoText.setString("Playing: Click Sound");
                }
                if (jumpButton.getGlobalBounds().contains(mousePos)) {
                    jumpSound.play();
                    infoText.setString("Playing: Jump Sound");
                }
                if (coinButton.getGlobalBounds().contains(mousePos)) {
                    coinSound.play();
                    infoText.setString("Playing: Coin Sound");
                }
            }

            // 키보드 입력
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    clickSound.play();
                    infoText.setString("Playing: Click Sound (Keyboard)");
                }
                if (keyPressed->code == sf::Keyboard::Key::J) {
                    jumpSound.play();
                    infoText.setString("Playing: Jump Sound (Keyboard)");
                }
                if (keyPressed->code == sf::Keyboard::Key::C) {
                    coinSound.play();
                    infoText.setString("Playing: Coin Sound (Keyboard)");
                }
            }
        }

        // 사운드 상태 확인 및 업데이트
        // sf::Sound::Status::Playing, Paused, Stopped
        std::string statusText = "Status: ";
        if (clickSound.getStatus() == sf::Sound::Status::Playing) {
            statusText += "Click playing... ";
        }
        if (jumpSound.getStatus() == sf::Sound::Status::Playing) {
            statusText += "Jump playing... ";
        }
        if (coinSound.getStatus() == sf::Sound::Status::Playing) {
            statusText += "Coin playing... ";
        }
        if (clickSound.getStatus() == sf::Sound::Status::Stopped &&
            jumpSound.getStatus() == sf::Sound::Status::Stopped &&
            coinSound.getStatus() == sf::Sound::Status::Stopped) {
            statusText += "Ready";
        }

        infoText.setString(statusText);

        // 렌더링
        window.clear(sf::Color::White);
        window.draw(clickButton);
        window.draw(jumpButton);
        window.draw(coinButton);
        window.draw(titleText);
        window.draw(clickText);
        window.draw(jumpText);
        window.draw(coinText);
        window.draw(infoText);
        window.draw(instructionText);
        window.display();
    }

    return 0;
}
