#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Music Control - SFML 3.0
//
// sf::Music vs sf::Sound:
// - Music: 큰 파일 스트리밍 (배경음악) - 메모리 효율적
// - Sound: 짧은 파일 전체 로드 (효과음) - 빠른 재생
//
// Music 주요 기능:
// - play(), pause(), stop()
// - setLoop(true/false) - 반복 재생
// - setPlayingOffset() - 재생 위치 설정
// - getPlayingOffset() - 현재 재생 위치
//
// 음악 파일 준비:
// assets/ 폴더에 music.ogg 또는 music.wav 추가
// 무료 음악: incompetech.com, freemusicarchive.org

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Music Control - SFML 3.0");
    window.setFramerateLimit(60);

    // Music 객체 생성 및 로드
    sf::Music music;

    // SFML 3.0에서는 openFromFile 사용
    if (!music.openFromFile("assets/bgmusic.wav")) {
        std::cout << "Warning: bgmusic.wav not found in assets/ folder\n";
        std::cout << "Try bgmusic.ogg or bgmusic.mp3\n";
        std::cout << "Free music: incompetech.com, freemusicarchive.org\n";

        // 다른 파일 형식 시도
        if (!music.openFromFile("assets/bgmusic.ogg")) {
            if (!music.openFromFile("assets/bgmusic.mp3")) {
                std::cout << "No music file found. Continue without music.\n";
            }
        }
    }

    // 기본 설정
    music.setLooping(true);  // 반복 재생 - SFML 3.0
    music.setVolume(50);  // 볼륨 50%

    // UI 설정
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // 버튼 생성
    sf::RectangleShape playButton({150, 50});
    playButton.setPosition({100, 200});
    playButton.setFillColor(sf::Color::Green);

    sf::RectangleShape pauseButton({150, 50});
    pauseButton.setPosition({280, 200});
    pauseButton.setFillColor(sf::Color::Yellow);

    sf::RectangleShape stopButton({150, 50});
    stopButton.setPosition({460, 200});
    stopButton.setFillColor(sf::Color::Red);

    sf::RectangleShape loopButton({150, 50});
    loopButton.setPosition({100, 300});
    loopButton.setFillColor(sf::Color::Cyan);

    // 텍스트
    sf::Text titleText(font, "Music Player Controls", 28);
    titleText.setPosition({250, 50});

    sf::Text playText(font, "Play", 18);
    playText.setPosition({145, 215});

    sf::Text pauseText(font, "Pause", 18);
    pauseText.setPosition({315, 215});
    pauseText.setFillColor(sf::Color::Black);

    sf::Text stopText(font, "Stop", 18);
    stopText.setPosition({505, 215});

    sf::Text loopText(font, "Loop: ON", 18);
    loopText.setPosition({130, 315});
    loopText.setFillColor(sf::Color::Black);

    sf::Text statusText(font, "Status: Stopped", 16);
    statusText.setPosition({100, 400});

    sf::Text timeText(font, "Time: 0:00", 16);
    timeText.setPosition({100, 430});

    sf::Text instructionText(font,
        "P: Play    Space: Pause    S: Stop    L: Toggle Loop\n"
        "Up/Down: Volume", 14);
    instructionText.setPosition({100, 500});

    while (window.isOpen()) {
        // 이벤트 처리
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // 마우스 클릭
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                                     static_cast<float>(mousePressed->position.y));

                if (playButton.getGlobalBounds().contains(mousePos)) {
                    music.play();
                }
                if (pauseButton.getGlobalBounds().contains(mousePos)) {
                    music.pause();
                }
                if (stopButton.getGlobalBounds().contains(mousePos)) {
                    music.stop();
                }
                if (loopButton.getGlobalBounds().contains(mousePos)) {
                    music.setLooping(!music.isLooping());  // 토글 - SFML 3.0
                    loopText.setString(music.isLooping() ? "Loop: ON" : "Loop: OFF");
                }
            }

            // 키보드 입력
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::P) {
                    music.play();
                }
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    music.pause();
                }
                if (keyPressed->code == sf::Keyboard::Key::S) {
                    music.stop();
                }
                if (keyPressed->code == sf::Keyboard::Key::L) {
                    music.setLooping(!music.isLooping());
                    loopText.setString(music.isLooping() ? "Loop: ON" : "Loop: OFF");
                }
                // 볼륨 조절
                if (keyPressed->code == sf::Keyboard::Key::Up) {
                    float volume = music.getVolume();
                    music.setVolume(std::min(100.f, volume + 10));
                }
                if (keyPressed->code == sf::Keyboard::Key::Down) {
                    float volume = music.getVolume();
                    music.setVolume(std::max(0.f, volume - 10));
                }
            }
        }

        // 상태 업데이트
        sf::Music::Status status = music.getStatus();
        if (status == sf::Music::Status::Playing) {
            statusText.setString("Status: Playing");
            statusText.setFillColor(sf::Color::Green);
        } else if (status == sf::Music::Status::Paused) {
            statusText.setString("Status: Paused");
            statusText.setFillColor(sf::Color::Yellow);
        } else {
            statusText.setString("Status: Stopped");
            statusText.setFillColor(sf::Color::Red);
        }

        // 재생 시간 표시
        sf::Time playingTime = music.getPlayingOffset();
        int minutes = static_cast<int>(playingTime.asSeconds()) / 60;
        int seconds = static_cast<int>(playingTime.asSeconds()) % 60;
        timeText.setString("Time: " + std::to_string(minutes) + ":" +
                          (seconds < 10 ? "0" : "") + std::to_string(seconds) +
                          " | Volume: " + std::to_string(static_cast<int>(music.getVolume())) + "%");

        // 렌더링
        window.clear(sf::Color::White);
        window.draw(playButton);
        window.draw(pauseButton);
        window.draw(stopButton);
        window.draw(loopButton);
        window.draw(titleText);
        window.draw(playText);
        window.draw(pauseText);
        window.draw(stopText);
        window.draw(loopText);
        window.draw(statusText);
        window.draw(timeText);
        window.draw(instructionText);
        window.display();
    }

    return 0;
}
