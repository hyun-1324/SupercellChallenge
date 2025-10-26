#include "PowerUp.h"
#include "Game.h"
#include "Constants.h"
#include <iostream>

PowerUp::PowerUp(Game* pGame, sf::Color colour) :
    Rectangle(sf::Vector2f(PowerUpWidth, PowerUpHeight)),
    m_pGame(pGame), colour(colour)
{
    int x = std::rand() % ScreenWidth;
    int y = std::rand() % ScreenHeight;
    pos = sf::Vector2f((float) x, (float) y);
    setPosition(pos);
    sprite.setTexture(*m_pGame->getApple());
    sprite.setScale(0.05, 0.05);
    sprite.setPosition(pos);
    sprite.setColor(colour);
    (colour == sf::Color::Red) ? type = SUPER_SPEED :
    (colour == sf::Color::Blue) ? type = INVINCIBLE :
    (colour == sf::Color::Yellow) ? type = GARLIC :
    type = GARLIC;
}

int PowerUp::steppedOn()
{
    if (collidesWith((Rectangle *) m_pGame->getPlayer(0)))
        return 0;
    else if (m_pGame->getState() == Game::State::ACTIVE_2P && collidesWith((Rectangle *) m_pGame->getPlayer(1)))
        return 1;
    return -1;
}

void PowerUp::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}