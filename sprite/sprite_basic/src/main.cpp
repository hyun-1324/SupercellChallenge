#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Sprite basic example");

    // 1. Texture 생성 및 로드
    sf::Texture texture;
    if (!texture.loadFromFile("../assets/player.png")) {
        std::cout << "Failed to load image!" << std::endl;
        return -1;
    }

    // 2. Sprite 생성 및 Texture 설정
    // 1. 기본 - 왼쪽 위가 (300, 300)에 위치
    sf::Sprite sprite(texture);
    sprite.setPosition({300, 300});

    // 2. Origin을 중심으로 변경 - 중심이 (300, 300)에 위치
    // sf::FloatRect bounds = sprite.getLocalBounds();
    // sprite.setOrigin(bounds.size / 2.f);
    // sprite.setPosition({300, 300});

    // 3. Origin을 오른쪽 아래로 변경 - 오른쪽 아래가 (300, 300)에 위치
    // sprite.setOrigin(bounds.size);
    // sprite.setPosition({300, 300});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
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


/*
  set* vs 그냥 함수 비교
  | 함수            | 타입  | 의미          | 동작          |
  |---------------|-----|-------------|-------------|
  | setPosition() | 절대  | "이 위치로!"    | 지정한 좌표로 이동  |
  | move()        | 상대  | "이만큼 움직여!"  | 현재 위치에서 더하기 |
  | setRotation() | 절대  | "이 각도로!"    | 지정한 각도로 설정  |
  | rotate()      | 상대  | "이만큼 더 돌려!" | 현재 각도에서 더하기 |
  | setScale()    | 절대  | "이 크기로!"    | 지정한 배율로 설정  |
  | scale()       | 상대  | "이만큼 곱해!"   | 현재 배율에 곱하기  |
  | setOrigin()   | 절대  | "기준점 설정"    | 변환의 중심점 설정  |
  */