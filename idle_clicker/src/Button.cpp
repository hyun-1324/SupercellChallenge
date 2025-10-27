#include "../include/Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, const std::string& text, const sf::Font& font)
    : m_text(font, text, 16)
    , m_isHovered(false)
    , m_enabled(true)
{
    m_shape.setSize(size);
    m_shape.setPosition(position);
    m_shape.setOutlineThickness(2);

    m_text.setPosition({position.x + 10, position.y + 10});
    m_text.setFillColor(sf::Color::White);

    updateColors();
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
    window.draw(m_text);
}

void Button::update(sf::Vector2i mousePos) {
    sf::Vector2f mousePosF(mousePos);
    m_isHovered = m_shape.getGlobalBounds().contains(mousePosF);
    updateColors();
}

bool Button::isClicked(sf::Vector2i mousePos) const {
    if (!m_enabled) return false;
    sf::Vector2f mousePosF(mousePos);
    return m_shape.getGlobalBounds().contains(mousePosF);
}

void Button::setText(const std::string& text) {
    m_text.setString(text);
}

void Button::setEnabled(bool enabled) {
    m_enabled = enabled;
    updateColors();
}

void Button::updateColors() {
    if (!m_enabled) {
        m_shape.setFillColor(sf::Color(50, 50, 50));
        m_shape.setOutlineColor(sf::Color(30, 30, 30));
    } else if (m_isHovered) {
        m_shape.setFillColor(sf::Color(80, 150, 80));
        m_shape.setOutlineColor(sf::Color::Green);
    } else {
        m_shape.setFillColor(sf::Color(60, 120, 60));
        m_shape.setOutlineColor(sf::Color(40, 100, 40));
    }
}
