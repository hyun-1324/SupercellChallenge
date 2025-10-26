#include "BloodPools.h"
#include "Game.h"

BloodPools::BloodPools(Game* game, sf::Vector2f position) :
	Vampire(game, position),
	m_pGame(game)
{
	m_sprite.setTexture(*m_pGame->getBloodPoolTexture());
    m_sprite.setScale(2.0f, 2.0f);
}

void BloodPools::update(float deltaTime)
{
    if (m_bloodSpawn && !m_isBloodPoolActive)
    {
        if (m_spawnClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            // Spawn the blood pool
            m_isBloodPoolActive = true;
            m_lifeClock.restart();
        }
    }
    else if (m_isBloodPoolActive)
    {
        if (m_lifeClock.getElapsedTime().asSeconds() >= 7.0f)
        {
            // Delete the blood pool
            m_isBloodPoolActive = false;
            m_bloodSpawn = false;
        }
    }
}

void BloodPools::onVampireKilled()
{
    m_bloodSpawn = true;
    m_spawnClock.restart();
}