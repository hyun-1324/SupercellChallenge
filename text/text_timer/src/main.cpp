#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

// 시간을 MM:SS.MS 형식으로 변환
std::string formatTime(float seconds) {
    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int millisecs = static_cast<int>((seconds - static_cast<int>(seconds)) * 100);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << secs << "."
       << std::setfill('0') << std::setw(2) << millisecs;

    return ss.str();
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Text - Timer");

    // 폰트 로드
    sf::Font font;
    if (!font.openFromFile("../assets/Arial.ttf")) {
        if (!font.openFromFile("../assets/font.ttc")) {
            std::cout << "폰트 로드 실패!" << std::endl;
            return -1;
        }
    }

    // 카운트다운 타이머 (30초)
    float countdownTime = 30.0f;
    sf::Clock countdownClock;

    sf::Text countdownTitle(font);
    countdownTitle.setString("Countdown Timer");
    countdownTitle.setCharacterSize(36);
    countdownTitle.setFillColor(sf::Color::Yellow);
    countdownTitle.setPosition({100, 50});

    sf::Text countdownText(font);
    countdownText.setCharacterSize(72);
    countdownText.setFillColor(sf::Color::Green);
    countdownText.setOutlineColor(sf::Color::Black);
    countdownText.setOutlineThickness(3);
    countdownText.setPosition({100, 120});

    // 스톱워치 (경과 시간)
    sf::Clock stopwatchClock;

    sf::Text stopwatchTitle(font);
    stopwatchTitle.setString("Stopwatch");
    stopwatchTitle.setCharacterSize(36);
    stopwatchTitle.setFillColor(sf::Color::Cyan);
    stopwatchTitle.setPosition({100, 300});

    sf::Text stopwatchText(font);
    stopwatchText.setCharacterSize(56);
    stopwatchText.setFillColor(sf::Color::White);
    stopwatchText.setPosition({100, 370});

    // 상태 텍스트
    sf::Text statusText(font);
    statusText.setCharacterSize(28);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition({100, 500});

    bool countdownFinished = false;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 카운트다운 타이머 업데이트
        if (!countdownFinished) {
            float elapsed = countdownClock.getElapsedTime().asSeconds();
            float remaining = countdownTime - elapsed;

            if (remaining <= 0) {
                remaining = 0;
                countdownFinished = true;
                statusText.setString("TIME'S UP!");
                statusText.setFillColor(sf::Color::Red);
                countdownText.setFillColor(sf::Color::Red);
            }

            // 10초 이하면 빨간색
            if (remaining <= 10.0f && !countdownFinished) {
                countdownText.setFillColor(sf::Color::Red);
            }

            countdownText.setString(formatTime(remaining));
        }

        // 스톱워치 업데이트
        float elapsed = stopwatchClock.getElapsedTime().asSeconds();
        stopwatchText.setString(formatTime(elapsed));

        window.clear(sf::Color::Black);
        window.draw(countdownTitle);
        window.draw(countdownText);
        window.draw(stopwatchTitle);
        window.draw(stopwatchText);

        if (countdownFinished) {
            window.draw(statusText);
        }

        window.display();
    }

    return 0;
}

/*
타이머 핵심:

1. 경과 시간 측정:
   sf::Clock clock;
   float elapsed = clock.getElapsedTime().asSeconds();

2. 카운트다운:
   float remaining = maxTime - elapsed;

3. 시간 포맷팅:
   std::stringstream으로 MM:SS 형식 변환

4. 조건부 색상:
   if (remaining < 10.0f) {
       text.setFillColor(sf::Color::Red);  // 경고!
   }

5. 게임잼 활용:
   - 제한 시간 게임
   - 스피드런 기록
   - 타임 어택 모드
*/
