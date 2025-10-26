#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Rectangle.h"

class Arrow : public Rectangle
{
public:
    Arrow(const sf::Vector2f& position, const sf::Vector2f& direction);
    void update(float deltaTime);
    bool isOffScreen() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Vector2f m_direction;
    float m_speed = 500.0f; // Adjust the speed as needed
};