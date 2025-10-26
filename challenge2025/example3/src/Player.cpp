#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>())
{
	PlayerSpeed = 155.0f;
    setOrigin(sf::Vector2f(0.0f, 0.0f));
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
	m_health = 1;
	PlayerSpeed = 155.0f;
	setAttackSpeed(1.25f);
    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    
    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;

    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;
    
    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
    setPosition(std::clamp(getPosition().x, 0.0f, (float)ScreenWidth), getPosition().y);

    if (inputData.m_movingLeft && !inputData.m_movingRight)
	{
        m_direction = LEFT;
		old_direction = LEFT;
	}
    else if (!inputData.m_movingLeft && inputData.m_movingRight)
	{
        m_direction = RIGHT;
		old_direction = RIGHT;
	}
    else if (inputData.m_movingUp && !inputData.m_movingDown)
        m_direction = UP;
    else if (!inputData.m_movingUp && inputData.m_movingDown)
        m_direction = DOWN;
	else if (inputData.m_movingUp && inputData.m_movingLeft)
		m_direction = UP_LEFT;
	else if (inputData.m_movingUp && inputData.m_movingRight)
		m_direction = UP_RIGHT;
	else if (inputData.m_movingDown && inputData.m_movingLeft)
		m_direction = DOWN_LEFT;
	else if (inputData.m_movingDown && inputData.m_movingRight)
		m_direction = DOWN_RIGHT;
}

void Player::attack()
{
    m_pWeapon->setActive(true);
}

void Player::update(float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();

    m_sprite.setPosition(getPosition());
	if (m_direction == LEFT || m_direction == UP_LEFT || m_direction == DOWN_LEFT) {
		m_pWeapon->setPosition(sf::Vector2f(
			getCenter().x - weaponSize.x,
			getCenter().y - weaponSize.y / 2.0f));
	} else if (m_direction == RIGHT || m_direction == UP_RIGHT || m_direction == DOWN_RIGHT) {
		m_pWeapon->setPosition(sf::Vector2f(
			getCenter().x,
			getCenter().y - weaponSize.y / 2.0f));
	}
	else if (m_direction == UP || m_direction == DOWN) {
		if (old_direction == LEFT) {
			m_pWeapon->setPosition(sf::Vector2f(
				getCenter().x - weaponSize.x,
				getCenter().y - weaponSize.y / 2.0f));
		} else if (old_direction == RIGHT) {
			m_pWeapon->setPosition(sf::Vector2f(
				getCenter().x,
				getCenter().y - weaponSize.y / 2.0f));
		}
	}
	m_pWeapon->update(deltaTime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
}

void Player::setIsDead(bool isDead)
{
	if (isDead)
	{
		m_health--;
		if (m_health <= 0)
		{
			m_isDead = isDead;
		}
	}
	else
		m_isDead = isDead;
}

void Player::setAttackSpeed(float attackSpeed)
{
	m_pGame->getGameInput()->setAttackCooldown(attackSpeed);
}

float Player::getAttackSpeed()
{
	return m_pGame->getGameInput()->getAttackCooldown();
}