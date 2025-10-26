#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Sprite Sheet");

    // 스프라이트 시트 로드
    sf::Texture spriteSheet;
    if (!spriteSheet.loadFromFile("assets/spritesheet.png")) {
        return -1;
    }

    sf::Sprite sprite(spriteSheet);
    sprite.setPosition({400, 300});

    // 스프라이트 시트에서 특정 부분만 선택
    // IntRect({left, top}, {width, height})
    sprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));  // 첫 번째 32x32 영역

    int currentFrame = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // 스페이스바로 프레임 변경
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    currentFrame = (currentFrame + 1) % 4;  // 4개 프레임 순환
                    sprite.setTextureRect(sf::IntRect({currentFrame * 32, 0}, {32, 32}));
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }

    return 0;
}