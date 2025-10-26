#pragma once

#include "Rectangle.h"

class Weapon : public Rectangle
{
public:
    Weapon();
    virtual ~Weapon() {}

    void setActive(bool isActive);
    void update(float deltaTime);
    bool isActive() { return m_isActive; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void reset();

	Weapon* getWeapon() { return this; }
	void setWidth(float width) { WeaponWidth = width; }
	float getWidth() { return WeaponWidth; }

private:
    bool m_isActive = false;
    float m_timer = 0.0f;
	float WeaponWidth = 100.0f;
};