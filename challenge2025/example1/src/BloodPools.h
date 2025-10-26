#pragma once

#include "Rectangle.h"
#include "Vampire.h"
#include <SFML/System/Clock.hpp>

class Game;
/*
	the idea is that when a vampire is killed, a blood pool is spawned after 1 second and it will last 5 seconds. if a vampire walks over the blood pool, it will moves at a faster pace for 2-3 seconds
*/
class BloodPools : public Vampire
{
public:
	BloodPools(Game* game, sf::Vector2f position);
	virtual ~BloodPools() {}
	void update(float deltaTime);
	void onVampireKilled();

private:
	Game* m_pGame;
	bool m_bloodSpawn = false;
	sf::Clock m_spawnClock;
	sf::Clock m_lifeClock;
	bool m_isBloodPoolActive = false;
};
