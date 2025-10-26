#pragma once

#include <SFML/Window/Keyboard.hpp>

class Game;
class Player;

struct InputData
{
    bool m_movingUp = false;
    bool m_movingDown = false;
    bool m_movingLeft = false;
    bool m_movingRight = false;

    bool hasInputs() { return m_movingUp || m_movingDown || m_movingLeft || m_movingRight; }
};

class GameInput
{
public:
    GameInput(Game* pGame, Player* pPlayer);
    ~GameInput();
    
    void update(float deltaTime);
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);
	void	setAttackCooldown(float attackCooldown) { m_attackCooldown = attackCooldown; }
	float	getAttackCooldown() { return m_attackCooldown; }
    
    
private:
    InputData m_inputData;
	float m_timeSinceLastAttack = 0.0f;
	//Hello. If you are reading this. Yes I just googled the whip timer in vampire survivors lol 1.35 seconds. Although I think 1.25 is a smidge nicer.
	float m_attackCooldown = 1.25f;
    Game* m_pGame;
    Player* m_pPlayer;
};
