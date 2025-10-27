#include "../include/Item.h"

Item::Item(int level, sf::Vector2f position)
    : m_level(level)
    , m_gridPos(-1, -1)
    , m_isDragging(false)
{
    m_shape.setSize({60, 60});
    m_shape.setOrigin({30, 30});
    m_shape.setPosition(position);
    m_shape.setOutlineThickness(2);
    m_shape.setOutlineColor(sf::Color::White);

    updateColor();
}

void Item::updateColor() {
    // 레벨별 색상 (무지개색)
    switch (m_level) {
        case 1: m_shape.setFillColor(sf::Color(255, 200, 200)); break;  // 연한 빨강
        case 2: m_shape.setFillColor(sf::Color(255, 220, 150)); break;  // 주황
        case 3: m_shape.setFillColor(sf::Color(255, 255, 150)); break;  // 노랑
        case 4: m_shape.setFillColor(sf::Color(150, 255, 150)); break;  // 초록
        case 5: m_shape.setFillColor(sf::Color(150, 200, 255)); break;  // 파랑
        case 6: m_shape.setFillColor(sf::Color(200, 150, 255)); break;  // 보라
        case 7: m_shape.setFillColor(sf::Color(255, 150, 255)); break;  // 분홍
        default: m_shape.setFillColor(sf::Color::White); break;
    }
}

void Item::setPosition(sf::Vector2f position) {
    m_shape.setPosition(position);
}

void Item::setGridPosition(int x, int y) {
    m_gridPos = {x, y};
}

void Item::draw(sf::RenderWindow& window) const {
    // Draw shape (brighter if dragging)
    sf::RectangleShape drawShape = m_shape;
    if (m_isDragging) {
        drawShape.setOutlineThickness(4);
        drawShape.setOutlineColor(sf::Color::Yellow);
        // Make it slightly transparent when dragging
        sf::Color color = drawShape.getFillColor();
        color.a = 200;
        drawShape.setFillColor(color);
    }
    window.draw(drawShape);

    // 레벨 숫자 표시
    sf::Font font;
    if (font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        sf::Text text(font, std::to_string(m_level), 32);
        text.setPosition(m_shape.getPosition());
        text.setOrigin({text.getGlobalBounds().size.x / 2, text.getGlobalBounds().size.y / 2 + 8});
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        window.draw(text);
    }
}
