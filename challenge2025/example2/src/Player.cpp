#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include <iostream>
#include "Game.h"

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>())
{
    static int newId = 0;
    this->id = newId ++;
    sf::Vector2f pos = this->getPosition();
    setOrigin(sf::Vector2f(0,0));
    powerUpBar = sf::RectangleShape(sf::Vector2f(80, 6));
    powerUpBar.setFillColor(sf::Color::Transparent);
    powerUpBar.setPosition(pos.x - POWER_BAR_OFFSET, pos.y - POWER_BAR_OFFSET);
    powerUpBackground = sf::RectangleShape(sf::Vector2f(80, 6));
    powerUpBackground.setFillColor(sf::Color::Transparent);
    powerUpBackground.setOutlineThickness(2);
    powerUpBackground.setOutlineColor(sf::Color::Transparent);
    powerUpBackground.setPosition(pos.x - POWER_BAR_OFFSET, pos.y - POWER_BAR_OFFSET);
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    setIsDead(false);
    if (id == 0)
        setPosition(400, 800);
    else if (id == 1)
        setPosition(1200, 800);
    float scale = (this->getPosition().y / (float) ScreenHeight) + 3.0f;
    m_sprite.setScale(scale, scale);
    m_sprite.setPosition(this->getPosition());
    m_sprite.setOrigin(0, 0);
    if (id == 1)
        m_sprite.setColor(sf::Color::Red);
    setHitPoints(6);
    setImmunity(0);
    message.setCharacterSize(14);
    message.setFont(*(m_pGame->getFont()));
    message.setStyle(sf::Text::Bold);
    message.setPosition(this->getPosition().x - message.getString().getSize(), this->getPosition().y - 40);
    m_messageTimer = 0;
    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    float speedBoost = 1.0f;

    if (m_superSpeedTimer > 0)
        speedBoost = 3.0f;
    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;
    xSpeed *= speedBoost;
    m_changedDirection = (m_direction == RIGHT && inputData.m_movingLeft) || (m_direction == LEFT && inputData.m_movingRight);

    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;
    ySpeed *= speedBoost;
    
    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
    setPosition(std::clamp(this->getPosition().x, 0.0f, ((float)ScreenWidth - PlayerWidth)), std::clamp(this->getPosition().y, 0.0f, ((float)ScreenHeight) - PlayerHeight));
    if (m_pGame->checkCollisionWithVampires(this))
        sf::Transformable::move(sf::Vector2f(-xSpeed, -ySpeed));
    if (m_pWeapon->isActive() == false)
    {
        if (inputData.m_movingLeft == true && inputData.m_movingRight == false)
            m_direction = LEFT;
        else if (inputData.m_movingLeft == false && inputData.m_movingRight == true)
            m_direction = RIGHT;
    }
}

void Player::attack()
{
    m_pWeapon->setActive(true);
    std::srand(std::time(NULL));
    int n = std::rand() % 3;
    (n == 0) ? setMessage("\"How d'you like them apples?\"", sf::Color::White) :
    (n == 1) ? setMessage("\"Take that!\"", sf::Color::White) :
    setMessage("\"Haaiiyyah!\"", sf::Color::White);
}

void Player::update(float deltaTime)
{
    float scaleFactor = 3.0f + (this->getPosition().y / ScreenHeight);
    if (m_hitPoints == 0) {
        setIsDead(true);
        return ;
    }

    decrementTimers(deltaTime);
    
    handlePowerUpBar();
    sf::Vector2f weaponSize = m_pWeapon->getSize();
    float newX;
    float offset;
    (m_direction == RIGHT) ? offset = PlayerWidth : offset = 0;
    offset *= m_sprite.getGlobalBounds().getSize().x / (float) PlayerWidth;
    newX = this->getPosition().x + offset;
    float newY = this->getPosition().y;
    m_sprite.setPosition(newX, newY);
    (m_direction == RIGHT) ? m_sprite.setScale(-scaleFactor, scaleFactor) : m_sprite.setScale(scaleFactor, scaleFactor);
    m_pWeapon->setPosition(sf::Vector2f(
        getCenter().x - (m_direction == LEFT ? weaponSize.x : 0.0f),
        getCenter().y - weaponSize.y / 2.0f));
    m_pWeapon->update(deltaTime);
    message.setPosition(newX - message.getString().getSize() - offset, newY - 40);
}

void Player::drawHealthBar(sf::RenderTarget &target) const
{
    sf::Sprite heart1((m_hitPoints > 1) ? *m_pGame->getFullHeart() :
                    (m_hitPoints == 1) ? *m_pGame->getHalfHeart() : 
                    *m_pGame->getEmptyHeart());
    sf::Sprite heart2((m_hitPoints > 3) ? *m_pGame->getFullHeart() :
                    (m_hitPoints == 3) ? *m_pGame->getHalfHeart() :
                    *m_pGame->getEmptyHeart());
    sf::Sprite heart3((m_hitPoints == 6) ? *m_pGame->getFullHeart() :
                    (m_hitPoints == 5) ? *m_pGame->getHalfHeart() :
                    *m_pGame->getEmptyHeart());
    if (id == 0) {
        heart1.setPosition(sf::Vector2f(30, 30));
        heart2.setPosition(sf::Vector2f(100, 30));
        heart3.setPosition(sf::Vector2f(170, 30));
    }
    else {
        heart1.setPosition(sf::Vector2f(1330, 30));
        heart2.setPosition(sf::Vector2f(1400, 30));
        heart3.setPosition(sf::Vector2f(1470, 30));
    }
    heart1.setScale(0.1, 0.1);
    heart2.setScale(0.1, 0.1);
    heart3.setScale(0.1, 0.1);
    target.draw(heart1);
    target.draw(heart2);
    target.draw(heart3);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
    if (m_messageTimer > 0)
        target.draw(message);
    if (m_pGame->getState() != Game::State::WAITING)
        drawHealthBar(target);
    target.draw(powerUpBackground, states);
    target.draw(powerUpBar, states);
}

void Player::setMessage(std::string str, sf::Color colour)
{
    message.setString(str);
    message.setFillColor(colour);
    m_messageTimer = 3.f;
}

void Player::beHit()
{
    if (!m_immunity && m_invincible == 0.0f)
    {
        m_hitPoints --;
        setImmunity(2.f);
        std::srand(std::time(NULL));
        int n = std::rand() % 3;
        (n == 0) ? setMessage("\"Ow, that hurt!\"", sf::Color::White) :
        (n == 1) ? setMessage("\"Watch it!\"", sf::Color::White) :
        setMessage("\"For f***'s sake..\"", sf::Color::White);
    }
}

void Player::receivePowerUp(eType type)
{
    switch (type)
    {
        case (SUPER_SPEED):
            m_superSpeedTimer += PowerUpTime;
            if (m_superSpeedTimer > PowerUpTime)
                m_superSpeedTimer = PowerUpTime;
            setMessage("Super speed!", sf::Color::Red);
            break;
        case (INVINCIBLE):
            m_invincible += PowerUpTime;
            if (m_invincible > PowerUpTime)
                m_invincible = PowerUpTime;
            setMessage("Invincible!", sf::Color::Blue);
            break;
        case (GARLIC):
            m_garlicBreath += PowerUpTime;
            if (m_garlicBreath > PowerUpTime)
                m_garlicBreath = PowerUpTime;
            setMessage("Garlic breath!", sf::Color::Yellow);
            break;
    }
}

void Player::decrementTimers(float deltaTime)
{
    m_immunity -= deltaTime;
    if (m_immunity < 0)
        m_immunity = 0;
    m_messageTimer -= deltaTime;
    if (m_messageTimer < 0)
        m_messageTimer = 0;
    m_superSpeedTimer -= deltaTime;
    if (m_superSpeedTimer < 0)
        m_superSpeedTimer = 0;
    m_invincible -= deltaTime;
    if (m_invincible < 0)
        m_invincible = 0;
    m_garlicBreath -= deltaTime;
    if (m_garlicBreath < 0)
        m_garlicBreath = 0;
}

bool Player::powerUpActive()
{
    return m_invincible || m_superSpeedTimer || m_garlicBreath;
}

void Player::handlePowerUpBar()
{
    sf::Vector2f pos = this->getPosition();
    if (!powerUpActive())
    {
        powerUpBar.setFillColor(sf::Color::Transparent);
        powerUpBackground.setOutlineColor(sf::Color::Transparent);
        return ;
    }
    float power;
    (m_superSpeedTimer > m_invincible && m_superSpeedTimer > m_garlicBreath) ? power = m_superSpeedTimer :
    (m_invincible > m_garlicBreath) ? power = m_invincible :
    power = m_garlicBreath;
    powerUpBar.setFillColor(sf::Color::White);
    powerUpBackground.setOutlineColor(sf::Color::White);
    powerUpBar.setSize(sf::Vector2f((power / PowerUpTime) * 80, 6));
    powerUpBar.setPosition(pos.x - POWER_BAR_OFFSET, pos.y - POWER_BAR_OFFSET);
    powerUpBackground.setPosition(pos.x - POWER_BAR_OFFSET, pos.y - POWER_BAR_OFFSET);

}