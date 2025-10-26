#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Sprite Transform example");

    sf::Texture texture;
    if (!texture.loadFromFile("../assets/player.png")) {
        std::cout << "이미지 로드 실패!" << std::endl;
        return -1;
    }

    sf::Sprite sprite1(texture);
    std::cout << "회전 전 크기: "
              << sprite1.getGlobalBounds().size.x << " x "
              << sprite1.getGlobalBounds().size.y << std::endl;

    sprite1.setPosition({150, 300});
    sprite1.setScale({3.0f, 3.0f});  // 3배 확대
    sprite1.setOrigin({0.0f, 0.0f});  // 왼쪽 위 기준
    sprite1.setRotation(sf::degrees(45));

    std::cout << "회전 후 크기 (sprite1): "
              << sprite1.getGlobalBounds().size.x << " x "
              << sprite1.getGlobalBounds().size.y << std::endl;

    sf::Sprite sprite2(texture);
    sprite2.setPosition({500, 300});
    sprite2.setScale({3.0f, 3.0f});  // 3배 확대
    sf::FloatRect bounds = sprite2.getLocalBounds();
    sprite2.setOrigin(bounds.size / 2.f);  // 중심 기준
    sprite2.setRotation(sf::degrees(45));

    std::cout << "회전 후 크기 (sprite2): "
              << sprite2.getGlobalBounds().size.x << " x "
              << sprite2.getGlobalBounds().size.y << std::endl;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(sprite1);
        window.draw(sprite2);
        window.display();
    }

    return 0;
}

/*
// ❌ 잘못된 방법 - Texture가 scope 밖으로 나가면 안 됨!
sf::Sprite createSprite() {
    sf::Texture texture;  // 함수 끝나면 사라짐!
    texture.loadFromFile("image.png");
    sf::Sprite sprite(texture);  // SFML 3.0: 생성자에서 텍스처 설정
    return sprite;  // 위험! 흰 사각형만 보임
}

// ✅ 올바른 방법 1 - Texture를 밖에서 관리
sf::Texture texture;  // 전역 또는 main에서
texture.loadFromFile("image.png");
sf::Sprite sprite(texture);  // SFML 3.0: 생성자에서 직접 설정

// ✅ 올바른 방법 2 - setTexture() 사용도 가능
sf::Sprite sprite2;
sprite2.setTexture(texture);
*/