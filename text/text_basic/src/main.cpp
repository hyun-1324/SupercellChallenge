#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Text - Basic");

    // 폰트 로드 (SFML 3.0)
    sf::Font font;
    if (!font.openFromFile("../assets/Arial.ttf")) {
        // Arial이 없으면 Helvetica 시도
        if (!font.openFromFile("../assets/font.ttc")) {
            std::cout << "폰트 로드 실패!" << std::endl;
            return -1;
        }
    }

    // 1. 기본 텍스트
    sf::Text title(font);
    title.setString("SFML Text Tutorial");
    title.setCharacterSize(48);  // 글자 크기
    title.setFillColor(sf::Color::White);
    title.setPosition({200, 50});

    // 2. 다양한 스타일
    sf::Text boldText(font);
    boldText.setString("Bold Text");
    boldText.setCharacterSize(32);
    boldText.setStyle(sf::Text::Bold);
    boldText.setFillColor(sf::Color::Yellow);
    boldText.setPosition({100, 150});

    sf::Text italicText(font);
    italicText.setString("Italic Text");
    italicText.setCharacterSize(32);
    italicText.setStyle(sf::Text::Italic);
    italicText.setFillColor(sf::Color::Cyan);
    italicText.setPosition({100, 200});

    sf::Text underlineText(font);
    underlineText.setString("Underlined Text");
    underlineText.setCharacterSize(32);
    underlineText.setStyle(sf::Text::Underlined);
    underlineText.setFillColor(sf::Color::Magenta);
    underlineText.setPosition({100, 250});

    // 3. 색상과 외곽선
    sf::Text outlineText(font);
    outlineText.setString("Outline Text");
    outlineText.setCharacterSize(40);
    outlineText.setFillColor(sf::Color::Green);
    outlineText.setOutlineColor(sf::Color::Black);
    outlineText.setOutlineThickness(3);
    outlineText.setPosition({100, 320});

    // 4. 회전과 크기 조절
    sf::Text rotatedText(font);
    rotatedText.setString("Rotated!");
    rotatedText.setCharacterSize(36);
    rotatedText.setFillColor(sf::Color::Red);
    rotatedText.setRotation(sf::degrees(-15));
    rotatedText.setPosition({500, 400});

    // 5. 동적으로 변하는 텍스트
    sf::Text dynamicText(font);
    dynamicText.setString("Frame: 0");
    dynamicText.setCharacterSize(24);
    dynamicText.setFillColor(sf::Color::White);
    dynamicText.setPosition({100, 500});

    int frameCount = 0;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 프레임마다 텍스트 업데이트
        frameCount++;
        dynamicText.setString("Frame: " + std::to_string(frameCount));

        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(boldText);
        window.draw(italicText);
        window.draw(underlineText);
        window.draw(outlineText);
        window.draw(rotatedText);
        window.draw(dynamicText);
        window.display();
    }

    return 0;
}

/*
핵심 개념:

1. 폰트 로드:
   sf::Font font;
   font.loadFromFile("font.ttf");

2. 텍스트 생성:
   sf::Text text(font);
   text.setString("Hello");

3. 주요 속성:
   - setCharacterSize(크기): 글자 크기
   - setFillColor(색상): 텍스트 색상
   - setStyle(스타일): Bold, Italic, Underlined
   - setOutlineColor/Thickness: 외곽선
   - setPosition/setRotation: 변환

4. 동적 텍스트:
   text.setString("Score: " + std::to_string(score));
*/
