#include "Vampire.h"
#include "Constants.h"
#include "Game.h"
#include "Weapon.h"
#include "Player.h"
#include "MathUtils.h"
#include <iostream>

Vampire::Vampire(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(VampireWidth, VampireHeight)),
    m_pGame(game)
{
    setPosition(position);
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    setIsKilled(false);
    closestPlayer = 0;
    m_sprite.setTexture(*m_pGame->getVampireTexture());
    float scale = 1.5f + (position.y / ScreenHeight);
    m_sprite.setScale(scale, scale);
}

float Vampire::distanceBetweenPoints(sf::Vector2f a, sf::Vector2f b)
{
    return std::sqrt((std::abs(a.x - b.x) * std::abs(a.x - b.x)) + (std::abs(a.y - b.y) * std::abs(a.y - b.y)));
}

void Vampire::update(float deltaTime)
{
    if (m_isKilled)
        return;
    
    Player* pPlayer1 = m_pGame->getPlayer(0);
    Player* pPlayer2 = (m_pGame->getState() == Game::State::ACTIVE_2P) ? m_pGame->getPlayer(1) : nullptr;

    if (collidesWith(pPlayer1->getWeapon()) && pPlayer1->getWeapon()->isActive())
    {
        setIsKilled(true);
        return;
    }
    if (pPlayer2 && collidesWith(pPlayer2->getWeapon()) && pPlayer2->getWeapon()->isActive())
    {
        setIsKilled(true);
        return;
    }
    if (collidesWith(pPlayer1))
        pPlayer1->beHit();
    if (pPlayer2 && collidesWith(pPlayer2))
        pPlayer2->beHit();

    sf::Vector2f playerCenter;
    if (m_pGame->getState() == Game::State::ACTIVE_1P)
        playerCenter = pPlayer1->getCenter();
    else
    {
        float distToP1 = distanceBetweenPoints(pPlayer1->getCenter(), getCenter());
        float distToP2 = distanceBetweenPoints(pPlayer2->getCenter(), getCenter());
        (distToP1 < distToP2) ? playerCenter = pPlayer1->getCenter() : playerCenter = pPlayer2->getCenter();
        (distToP1 < distToP2) ? closestPlayer = 0 : closestPlayer = 1;
    }
    sf::Vector2f direction = VecNormalized(playerCenter - getCenter());
    direction *= VampireSpeed * deltaTime;

    if (m_pGame->getState() == Game::State::ACTIVE_1P) {
        if (pPlayer1->getGarlicBreath() > 0)
            direction *= -1.0f;
    }
    else
    {
        if (pPlayer1->getGarlicBreath() > 0 && closestPlayer == 0)
            direction *= -1.0f;
        if (pPlayer2 && pPlayer2->getGarlicBreath() > 0 && closestPlayer == 1)
            direction *= -1.0f;
    }
    sf::Transformable::move(direction);
    if (m_pGame->checkCollisionWithVampires(this))
        sf::Transformable::move(direction * -1.f);
    if (collidesWith(pPlayer1)) {
        sf::Transformable::move(direction * -1.f);
        pPlayer1->beHit();
    }
    if (pPlayer2 && collidesWith(pPlayer2)) {
        sf::Transformable::move(direction * -1.f);
        pPlayer2->beHit();
    }
    m_sprite.setPosition(getPosition());
    float scale = 1.5f + (m_sprite.getPosition().y / ScreenHeight);
    m_sprite.setScale(scale, scale);
}