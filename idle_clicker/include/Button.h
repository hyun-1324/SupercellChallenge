#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& text, const sf::Font& font);

    void draw(sf::RenderWindow& window) const;
    void update(sf::Vector2i mousePos);
    bool isClicked(sf::Vector2i mousePos) const;

    void setText(const std::string& text);
    void setEnabled(bool enabled);
    bool isEnabled() const { return m_enabled; }

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    bool m_isHovered;
    bool m_enabled;

    void updateColors();
};
