#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "DeltaTime - Basic Comparison");

    // FPS 제한 없음 - 컴퓨터 성능에 따라 다르게 실행됨
    // window.setFramerateLimit(60);  // 주석 처리!

    // DeltaTime 없이 움직이는 사각형 (빨간색)
    sf::RectangleShape noDeltaBox({50, 50});
    noDeltaBox.setFillColor(sf::Color::Red);
    noDeltaBox.setPosition({100, 200});

    // DeltaTime 적용한 사각형 (초록색)
    sf::RectangleShape deltaBox({50, 50});
    deltaBox.setFillColor(sf::Color::Green);
    deltaBox.setPosition({100, 350});

    // 속도 설정
    float speedWithoutDelta = 5.0f;      // 픽셀/프레임 (프레임 의존적!)
    float speedWithDelta = 200.0f;       // 픽셀/초 (시간 기반!)

    sf::Clock clock;  // DeltaTime 측정용
    sf::Clock fpsTimer;
    int frameCount = 0;
    float fps = 0;

    while (window.isOpen()) {
        // DeltaTime 계산 (이전 프레임 이후 경과 시간)
        float deltaTime = clock.restart().asSeconds();

        // FPS 계산
        frameCount++;
        if (fpsTimer.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount / fpsTimer.restart().asSeconds();
            frameCount = 0;
            std::cout << "FPS: " << fps << std::endl;
        }

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 1. DeltaTime 없이 움직임 (프레임 의존적)
        noDeltaBox.move({speedWithoutDelta, 0});  // 매 프레임마다 5픽셀

        // 2. DeltaTime 적용 움직임 (시간 기반)
        deltaBox.move({speedWithDelta * deltaTime, 0});  // 매 초당 200픽셀

        // 화면 끝에 도달하면 처음으로
        if (noDeltaBox.getPosition().x > 800) {
            noDeltaBox.setPosition({0, 200});
        }
        if (deltaBox.getPosition().x > 800) {
            deltaBox.setPosition({0, 350});
        }

        window.clear(sf::Color::Black);

        // 설명 텍스트 (간단히 도형으로 표시)
        window.draw(noDeltaBox);
        window.draw(deltaBox);

        window.display();
    }

    return 0;
}

/*
결과:
- 빨간 사각형 (DeltaTime 없음): FPS에 따라 속도가 달라짐
  60 FPS: 5 * 60 = 300 픽셀/초
  30 FPS: 5 * 30 = 150 픽셀/초  ← 절반 속도!

- 초록 사각형 (DeltaTime 적용): 항상 200 픽셀/초
  60 FPS: 200 * (1/60) = 3.33 픽셀/프레임
  30 FPS: 200 * (1/30) = 6.67 픽셀/프레임  ← 자동 조절!

핵심:
  movement = speed * deltaTime

  - speed: 픽셀/초 단위
  - deltaTime: 이전 프레임 이후 경과 시간 (초)
  - 결과: 프레임 레이트와 무관하게 일정한 속도!
*/
