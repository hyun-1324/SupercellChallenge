#pragma once

#include "Rectangle.h"
#include "Weapon.h"
#include "Constants.h"

#include <memory>

struct InputData;

class Weapon;
class Game;
class GameHandler;

enum eDirection
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}
    
    bool initialise();
    void move(InputData inputData, float deltaTime);
    void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead);

    Weapon* getWeapon() { return m_pWeapon.get(); }

	void setSpeed(float speed) { PlayerSpeed = speed; }
	float getSpeed() { return PlayerSpeed; }
	void setHealth(int health) { m_health = health; }
	int getHealth() { return m_health; }

	void setAttackSpeed(float attackSpeed);
	float getAttackSpeed();
	void setWhipLength(float whipLength) { m_pWeapon->setWidth(whipLength); }
	float getWhipLength() { return m_pWeapon->getWidth(); }





private:
    bool    m_isDead = false;
	int    	m_health = 1;
    eDirection m_direction = LEFT;
    Game*   m_pGame;
    std::unique_ptr<Weapon> m_pWeapon;
	eDirection old_direction = LEFT;
	float PlayerSpeed;
};
