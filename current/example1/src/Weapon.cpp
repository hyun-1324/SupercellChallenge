#include "Weapon.h"
#include "Constants.h"

Weapon::Weapon(WeaponType type) : Rectangle(sf::Vector2f(0, 0))
{
    this->m_type = type;
    if (m_type == WeaponType::SWORD)
    {
        setPosition(sf::Vector2f(ScreenWidth * 0.5f, ScreenHeight * 0.5f));
        setOrigin(sf::Vector2f(0.0f, 0.0f));
        setColor(sf::Color::Blue);
    }
    else if (m_type == WeaponType::BOW)
    {
        setPosition(sf::Vector2f(ScreenWidth * 0.5f, ScreenHeight * 0.5f));
        setOrigin(sf::Vector2f(0.0f, 0.0f));
        setColor(sf::Color::Red);
    }
}

void Weapon::setActive(bool isActive)
{
    m_isActive = isActive;
    if (isActive)
    {
        setSize(sf::Vector2f(WeaponWidth, WeaponHeight));
        m_timer = WeaponActiveTime;
    }
    else
    {
        setSize(sf::Vector2f(0.0f, 0.0f));
        m_timer = 0.0f;
    }
}

void Weapon::update(float deltaTime)
{
    if (m_isActive)
    {
        m_timer -= deltaTime;
        if (m_timer <= 0.0f)
        {
            setActive(false);
        }
    }
}
/*
If i want to implement direction also to the weapons, I need to have an accessor from players to weapons, so in weapon.draw, I can also have the direction of the weapon which I can use switch() and depending on the direction, I can draw the weapon in the correct position.
*/
void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_type == WeaponType::SWORD)
    {
        // Draw the sword as a rectangle
        sf::RectangleShape graphicsRect(getSize());
        graphicsRect.setFillColor(getColor());
        graphicsRect.setPosition(getPosition());
        target.draw(graphicsRect, states);
    }
    else if (m_type == WeaponType::BOW)
    {
        if (m_isActive) {
            // Draw the bow as a cross
            sf::Vector2f position = getPosition();
            position.x -= 25.0f;
            // Horizontal part of the cross
            sf::RectangleShape crossHorizontal(sf::Vector2f(50.0f, 10.0f));
            crossHorizontal.setFillColor(sf::Color::Red);
            crossHorizontal.setOrigin(crossHorizontal.getSize() / 2.0f);
            crossHorizontal.setPosition(position);

            // Vertical part of the cross
            sf::RectangleShape crossVertical(sf::Vector2f(10.0f, 50.0f));
            crossVertical.setFillColor(sf::Color::Red);
            crossVertical.setOrigin(crossVertical.getSize() / 2.0f);
            crossVertical.setPosition(position);

            target.draw(crossHorizontal, states);
            target.draw(crossVertical, states);
        }
    }
}