#pragma once

#include "Rectangle.h"

enum class WeaponType
{
    SWORD,
    BOW
};

class Weapon : public Rectangle
{
public:
    Weapon(WeaponType);
    virtual ~Weapon() {}

    void setActive(bool isActive);
    void update(float deltaTime);
    bool isActive() { return m_isActive; }
    WeaponType getType() { return m_type; }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    bool m_isActive = false;
    float m_timer = 0.0f;
    WeaponType m_type;

    sf::RectangleShape m_crossHorizontal;
    sf::RectangleShape m_crossVertical;
};
