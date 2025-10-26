#pragma once

#include "Rectangle.h"

class Game;

enum eType
{
    SUPER_SPEED,
    INVINCIBLE,
    GARLIC,
};

class PowerUp: public Rectangle
{
    public:
        PowerUp(Game* pGame, sf::Color colour);
        virtual ~PowerUp() {};
        int steppedOn();
        eType getType() { return type; }
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Sprite sprite;
        sf::Vector2f pos;
        Game* m_pGame;
        sf::Color colour;
        eType type;

};