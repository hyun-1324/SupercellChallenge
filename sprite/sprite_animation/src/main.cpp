#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Animation");

    sf::Texture spriteSheet;
    if (!spriteSheet.loadFromFile("../assets/player.png")) {
        std::cout << "이미지 로드 실패!" << std::endl;
        return -1;
    }

    sf::Sprite sprite(spriteSheet);
    sprite.setPosition({400, 300});
    sprite.setScale({0.5, 0.5});

    int currentFrame = 0;
    int totalFrames = 4;
    int frameWidth = 256;
    int frameHeight = 1024;

    sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
    sprite.setOrigin({frameWidth / 2.f, frameHeight / 2.f});

    sf::Clock clock;
    float animationSpeed = 0.1;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() > animationSpeed) {
            currentFrame = (currentFrame + 1) % totalFrames;
            sprite.setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            clock.restart();
        }

        window.clear(sf::Color::Black);  // 검은색 배경으로 변경
        window.draw(sprite);
        window.display();

    }

    return 0;
}
