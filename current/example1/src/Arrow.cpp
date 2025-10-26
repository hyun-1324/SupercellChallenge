#include "Arrow.h"

Arrow::Arrow(const sf::Vector2f& position, const sf::Vector2f& direction) : Rectangle(sf::Vector2f(20.0f, 5.0f)),
     m_direction(direction)
{
    setColor(sf::Color::Yellow);
    setPosition(position);
    setRotation(atan2(direction.y, direction.x) * 180 / 3.14159f);
}

void Arrow::update(float deltaTime)
{
    move(m_direction * m_speed * deltaTime);
}

bool Arrow::isOffScreen() const
{
    sf::Vector2f position = getPosition();
    return position.x < 0 || position.x > ScreenWidth || position.y < 0 || position.y > ScreenHeight;
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RectangleShape arrowShape(sf::Vector2f(20.0f, 5.0f)); // Adjust the size as needed
    arrowShape.setFillColor(getColor());
    arrowShape.setPosition(getPosition());
    arrowShape.setRotation(getRotation());
    target.draw(arrowShape, states);
}