#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Multiple Sprite");

    sf::Texture texture;
    if (!texture.loadFromFile("../assets/player.png")) {
        std::cout << "이미지 로드 실패!" << std::endl;
        return -1;
    }

    std::vector<sf::Sprite> sprites;

    for (int i = 0; i < 5; ++i) {
        sf::Sprite sprite(texture);
        sprite.setPosition({100.f + i * 150.f, 200.f});
        sprite.setScale({0.5f + i * 0.2f, 0.5f + i * 0.2f});
        sprites.push_back(sprite);
    }

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        for (auto& sprite : sprites) {
            window.draw(sprite);
        }

        window.display();
    }

    return 0;
}
