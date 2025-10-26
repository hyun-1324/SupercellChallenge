#include <SFML/Graphics.hpp>
#include <iostream>

// 키보드 대소문자 테스트

int main() {
    sf::RenderWindow window(sf::VideoMode({400, 300}), "Key Test");

    sf::Font font;
    font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");

    sf::Text text(font, "Press 'J' or 'j'", 20);
    text.setPosition({100, 100});
    text.setFillColor(sf::Color::Black);

    sf::Text shiftText(font, "Shift: Not pressed", 16);
    shiftText.setPosition({100, 150});
    shiftText.setFillColor(sf::Color::Blue);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // J 키 감지 (대소문자 무관)
                if (keyPressed->code == sf::Keyboard::Key::J) {
                    std::cout << "J key pressed! ";

                    // Shift 눌렸는지 확인
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
                        std::cout << "WITH Shift (J)" << std::endl;
                        text.setString("You pressed: J (with Shift)");
                    } else {
                        std::cout << "WITHOUT Shift (j)" << std::endl;
                        text.setString("You pressed: j (without Shift)");
                    }
                }
            }
        }

        // 실시간 Shift 상태 표시
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
            shiftText.setString("Shift: PRESSED");
            shiftText.setFillColor(sf::Color::Red);
        } else {
            shiftText.setString("Shift: Not pressed");
            shiftText.setFillColor(sf::Color::Blue);
        }

        window.clear(sf::Color::White);
        window.draw(text);
        window.draw(shiftText);
        window.display();
    }

    return 0;
}
