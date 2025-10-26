#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Text - Score System");

    // 폰트 로드
    sf::Font font;
    if (!font.openFromFile("../assets/Arial.ttf")) {
        if (!font.openFromFile("../assets/font.ttc")) {
            std::cout << "폰트 로드 실패!" << std::endl;
            return -1;
        }
    }

    // 플레이어
    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({20, 20});
    player.setPosition({400, 300});

    // 동전들
    std::vector<sf::CircleShape> coins;
    for (int i = 0; i < 15; ++i) {
        sf::CircleShape coin(10);
        coin.setFillColor(sf::Color::Yellow);
        coin.setOrigin({10, 10});
        coin.setPosition({100.f + (i % 5) * 150.f, 100.f + (i / 5) * 150.f});
        coins.push_back(coin);
    }

    // 점수
    int score = 0;

    // 점수 텍스트
    sf::Text scoreText(font);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition({20, 20});

    // 안내 텍스트
    sf::Text guideText(font);
    guideText.setString("Use Arrow Keys - Collect Coins!");
    guideText.setCharacterSize(24);
    guideText.setFillColor(sf::Color::Cyan);
    guideText.setPosition({200, 550});

    // 게임 클리어 텍스트 (처음엔 숨김)
    sf::Text clearText(font);
    clearText.setString("GAME CLEAR!");
    clearText.setCharacterSize(72);
    clearText.setFillColor(sf::Color::Yellow);
    clearText.setOutlineColor(sf::Color::Red);
    clearText.setOutlineThickness(3);
    clearText.setStyle(sf::Text::Bold);

    // 중앙 정렬
    sf::FloatRect textBounds = clearText.getLocalBounds();
    clearText.setOrigin(textBounds.size / 2.f);
    clearText.setPosition({400, 300});

    float speed = 300.0f;
    sf::Clock clock;
    bool gameCleared = false;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (!gameCleared) {
            // 플레이어 이동
            sf::Vector2f movement(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) movement.x -= 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) movement.x += 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) movement.y -= 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) movement.y += 1.0f;

            player.move(movement * speed * deltaTime);

            // 화면 밖으로 나가지 않게
            sf::Vector2f pos = player.getPosition();
            if (pos.x < 20) pos.x = 20;
            if (pos.x > 780) pos.x = 780;
            if (pos.y < 20) pos.y = 20;
            if (pos.y > 580) pos.y = 580;
            player.setPosition(pos);

            // 동전 충돌 감지 (Iterator 방식)
            sf::FloatRect playerBounds = player.getGlobalBounds();

            for (auto it = coins.begin(); it != coins.end(); ) {
                if (playerBounds.findIntersection(it->getGlobalBounds()).has_value()) {
                    // 점수 증가
                    score += 10;
                    scoreText.setString("Score: " + std::to_string(score));

                    // 동전 삭제 (erase는 다음 iterator 반환)
                    it = coins.erase(it);
                } else {
                    // 충돌 없으면 다음으로
                    ++it;
                }
            }

            // 모든 동전 수집 완료
            if (coins.empty()) {
                gameCleared = true;
            }
        }

        window.clear(sf::Color::Black);

        // 동전 그리기
        for (const auto& coin : coins) {
            window.draw(coin);
        }

        window.draw(player);
        window.draw(scoreText);

        if (!gameCleared) {
            window.draw(guideText);
        } else {
            window.draw(clearText);
        }

        window.display();
    }

    return 0;
}

/*
점수 시스템 핵심:

1. 점수 변수:
   int score = 0;

2. 점수 텍스트:
   sf::Text scoreText(font);
   scoreText.setString("Score: " + std::to_string(score));

3. 충돌 감지 & 삭제 (Iterator 방식):
   for (auto it = coins.begin(); it != coins.end(); ) {
       if (충돌) {
           score += 10;
           it = coins.erase(it);  // erase는 다음 iterator 반환
       } else {
           ++it;
       }
   }

4. 게임잼 팁:
   - Iterator 방식: 명확하고 읽기 쉬움
   - 외곽선(outline)으로 가독성 향상
   - 큰 글씨로 중요한 정보 강조
*/
