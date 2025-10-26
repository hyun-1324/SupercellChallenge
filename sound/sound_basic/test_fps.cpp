#include <SFML/Graphics.hpp>
#include <iostream>

// FPS 제한 테스트

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "FPS Test");

    // 테스트: 주석 처리해보기
    window.setFramerateLimit(60);  // ← 이거 주석 처리하고 실행해보기

    sf::Font font;
    font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");

    sf::Text fpsText(font, "", 30);
    fpsText.setPosition({10, 10});
    fpsText.setFillColor(sf::Color::White);

    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // FPS 계산
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount / fpsClock.restart().asSeconds();
            frameCount = 0;
        }

        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

        window.clear(sf::Color::Black);
        window.draw(fpsText);
        window.display();
    }

    return 0;
}
