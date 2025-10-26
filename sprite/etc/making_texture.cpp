#include <SFML/Graphics.hpp>

int main() {
    // 테스트용 Texture 생성
    sf::RenderTexture renderTexture;
    renderTexture.resize({64, 64});

    // 간단한 캐릭터 그리기
    sf::CircleShape head(20);
    head.setFillColor(sf::Color::Yellow);
    head.setPosition({12, 10});

    sf::RectangleShape body(sf::Vector2f(20, 30));
    body.setFillColor(sf::Color::Blue);
    body.setPosition({22, 30});

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(head);
    renderTexture.draw(body);
    renderTexture.display();

    // Texture로 변환
    const sf::Texture& texture = renderTexture.getTexture();

    return 0;
}