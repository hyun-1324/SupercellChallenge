#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Screen Shake - SFML 3.0
//
// 화면 흔들림 효과:
// - 폭발, 충돌, 데미지 등에 사용
// - View를 랜덤하게 약간씩 이동
// - 시간이 지나면 점점 약해짐
// - 게임에 임팩트 추가!
//
// 구현 방법:
// 1. 흔들림 타이머 설정
// 2. 타이머가 0보다 크면 View를 랜덤 이동
// 3. 매 프레임 타이머 감소
// 4. 타이머 0되면 원위치ㅈ

float randomFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Screen Shake - SFML 3.0");
    window.setFramerateLimit(60);

    // View
    sf::View view = window.getDefaultView();
    sf::Vector2f viewCenter = view.getCenter();  // 원래 중심 저장

    // 화면 흔들림 변수
    float shakeTimer = 0.0f;
    float shakeIntensity = 10.0f;  // 흔들림 강도

    // 플레이어
    sf::CircleShape player(30);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({30, 30});
    player.setPosition({400, 300});

    // 적 (충돌 테스트용)
    sf::CircleShape enemy(25);
    enemy.setFillColor(sf::Color::Red);
    enemy.setOrigin({25, 25});
    enemy.setPosition({600, 300});

    // 폭발 이펙트 (시각적 피드백)
    sf::CircleShape explosion(50);
    explosion.setFillColor(sf::Color(255, 100, 0, 0));  // 투명 시작
    explosion.setOrigin({50, 50});
    float explosionTimer = 0.0f;

    // UI
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text titleText(font, "SCREEN SHAKE DEMO", 36);
    titleText.setPosition({200, 50});
    titleText.setFillColor(sf::Color::Yellow);

    sf::Text infoText(font, "", 16);
    infoText.setPosition({10, 150});
    infoText.setFillColor(sf::Color::White);

    sf::Text controlsText(font,
        "1: Weak Shake (0.2s)\n"
        "2: Medium Shake (0.5s)\n"
        "3: Strong Shake (1.0s)\n"
        "Space: Collision Test\n"
        "C: Continuous Shake", 18);
    controlsText.setPosition({50, 350});
    controlsText.setFillColor(sf::Color::Cyan);

    sf::Clock deltaClock;

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // 다양한 강도의 흔들림
                if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    shakeTimer = 0.2f;  // 약한 흔들림
                    std::cout << "Weak Shake!\n";
                }
                if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    shakeTimer = 0.5f;  // 중간 흔들림
                    std::cout << "Medium Shake!\n";
                }
                if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    shakeTimer = 1.0f;  // 강한 흔들림
                    std::cout << "Strong Shake!\n";
                }

                // 충돌 테스트
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    // 플레이어를 적 위치로 순간이동
                    player.setPosition(enemy.getPosition());
                    shakeTimer = 0.6f;
                    explosionTimer = 0.3f;
                    explosion.setPosition(player.getPosition());
                    std::cout << "COLLISION!\n";
                }

                // 연속 흔들림
                if (keyPressed->code == sf::Keyboard::Key::C) {
                    shakeTimer = 2.0f;  // 2초간 계속
                    std::cout << "Continuous Shake!\n";
                }
            }
        }

        // 플레이어 이동 (화살표 키)
        float speed = 200.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            player.move({-speed * deltaTime, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            player.move({speed * deltaTime, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            player.move({0, -speed * deltaTime});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            player.move({0, speed * deltaTime});
        }

        // 화면 흔들림 처리
        if (shakeTimer > 0) {
            shakeTimer -= deltaTime;

            // 랜덤 오프셋 계산
            float offsetX = randomFloat(-shakeIntensity, shakeIntensity);
            float offsetY = randomFloat(-shakeIntensity, shakeIntensity);

            // View 중심을 원래 위치 + 랜덤 오프셋으로 설정
            view.setCenter({viewCenter.x + offsetX, viewCenter.y + offsetY});
        } else {
            // 흔들림 끝 - 원래 위치로
            view.setCenter(viewCenter);
        }

        // 폭발 이펙트
        if (explosionTimer > 0) {
            explosionTimer -= deltaTime;
            float alpha = (explosionTimer / 0.3f) * 200;  // 서서히 투명
            explosion.setFillColor(sf::Color(255, 100, 0, static_cast<std::uint8_t>(alpha)));
            explosion.setScale({1.0f + (0.3f - explosionTimer) * 3, 1.0f + (0.3f - explosionTimer) * 3});
        }

        // 정보 업데이트
        std::string info = "Shake Timer: " + std::to_string(shakeTimer).substr(0, 4) + "s\n";
        info += "View Offset: ";
        if (shakeTimer > 0) {
            sf::Vector2f offset = view.getCenter() - viewCenter;
            info += "(" + std::to_string(static_cast<int>(offset.x)) + ", " +
                    std::to_string(static_cast<int>(offset.y)) + ")";
        } else {
            info += "(0, 0)";
        }
        infoText.setString(info);

        // 렌더링
        window.clear(sf::Color::Black);

        // View 적용 (흔들림 효과)
        window.setView(view);

        // 게임 오브젝트
        window.draw(player);
        window.draw(enemy);
        if (explosionTimer > 0) {
            window.draw(explosion);
        }

        // UI는 기본 View 사용 (고정)
        window.setView(window.getDefaultView());
        window.draw(titleText);
        window.draw(infoText);
        window.draw(controlsText);

        window.display();
    }

    return 0;
}
