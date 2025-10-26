#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include "Game.h"

Player::Player(Game* pGame, WeaponType weaponType) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame)
{
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    if (weaponType == WeaponType::SWORD)
    {
        m_pWeapon = std::make_unique<Weapon>(WeaponType::SWORD);
    }
    else if (weaponType == WeaponType::BOW)
    {
        m_pWeapon = std::make_unique<Weapon>(WeaponType::BOW);
    }
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
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

    if (m_pWeapon->isActive() == false)
    {
        if (inputData.m_movingLeft == true && inputData.m_movingRight == false)
            m_direction = LEFT;
        else if (inputData.m_movingLeft == false && inputData.m_movingRight == true)
            m_direction = RIGHT;
        else if (inputData.m_movingUp && !inputData.m_movingDown)
            m_direction = UP;
        else if (!inputData.m_movingUp && inputData.m_movingDown)
            m_direction = DOWN;
    }
}

void Player::attack()
{
    /*
        set weapon active to true
        shoot arrow if its bow
        swing if its sword
    */
    m_pWeapon->setActive(true);
    if (m_pWeapon->getType() == WeaponType::BOW)
        shootArrow();
}

void Player::shootArrow()
{
    sf::Vector2f direction;
    switch (m_direction)
    {
        case RIGHT:
            direction = sf::Vector2f(1.0f, 0.0f);
            break;
        case LEFT:
            direction = sf::Vector2f(-1.0f, 0.0f);
            break;
        case UP:
            direction = sf::Vector2f(0.0f, -1.0f);
            break;
        case DOWN:
            direction = sf::Vector2f(0.0f, 1.0f);
            break;
    }

    sf::Vector2f position = getCenter() + direction * (getSize().x / 2.0f);
    m_arrows.push_back(std::make_unique<Arrow>(position, direction));
}

// void Player::shootArrow()
// {
//     sf::Vector2f direction = (m_direction == RIGHT) ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
//     sf::Vector2f position = getCenter() + direction * (getSize().x / 2.0f);
//     m_arrows.push_back(std::make_unique<Arrow>(position, direction));
// }

void Player::update(float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();
    sf::Vector2f weaponPosition = getCenter();

    switch (m_direction)
    {
        case LEFT:
            weaponPosition.x -= weaponSize.x;
            weaponPosition.y -= weaponSize.y / 2.0f;
            break;
        case RIGHT:
            weaponPosition.x += weaponSize.x / 2.0f;
            weaponPosition.y -= weaponSize.y / 2.0f;
            break;
        case UP:
            weaponPosition.x -= weaponSize.x / 2.0f;
            weaponPosition.y -= weaponSize.y;
            break;
        case DOWN:
            weaponPosition.x -= weaponSize.x / 2.0f;
            weaponPosition.y += weaponSize.y / 2.0f;
            break;
    }
    m_sprite.setPosition(getPosition());
    m_pWeapon->setPosition(weaponPosition);
    m_pWeapon->update(deltaTime);
    // Update arrows
    for (auto& arrow : m_arrows)
    {
        arrow->update(deltaTime);
    }
    // Remove off-screen arrows
    m_arrows.erase(std::remove_if(m_arrows.begin(), m_arrows.end(),
        [](const std::unique_ptr<Arrow>& arrow) { return arrow->isOffScreen(); }),
        m_arrows.end());
}

// void Player::update(float deltaTime)
// {
//     sf::Vector2f weaponSize = m_pWeapon->getSize();
//     sf::Vector2f weaponPosition = getCenter();
    
//     if (m_pWeapon->getType() == WeaponType::SWORD)
//     {
//         weaponPosition.x -= (m_direction == LEFT ? weaponSize.x : 0.0f);
//         weaponPosition.y -= weaponSize.y / 2.0f;
//     }
//     else if (m_pWeapon->getType() == WeaponType::BOW)
//     {
//         weaponPosition.x += (m_direction == RIGHT ? weaponSize.x / 2.0f : -weaponSize.x / 5.0f);
//         weaponPosition.y -= weaponSize.y / 2.0f;
//     }

//     m_sprite.setPosition(getPosition());
//     m_pWeapon->setPosition(weaponPosition);
//     m_pWeapon->update(deltaTime);

//     //update arrows
//     for (auto& arrow : m_arrows)
//     {
//         arrow->update(deltaTime);
//     }
//     // Remove offscreen arrows
//     m_arrows.erase(std::remove_if(m_arrows.begin(), m_arrows.end(),
//         [](const std::unique_ptr<Arrow>& arrow) { return arrow->isOffScreen(); }),
//         m_arrows.end());
// }

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
    if (m_pWeapon->getType() == WeaponType::BOW)
    {
        for (const auto& arrow : m_arrows)
        {
            target.draw(*arrow, states);
        }
    }
}

void Player::setWeaponType(WeaponType weaponType)
{
    m_pWeapon.reset();
    if (weaponType == WeaponType::SWORD)
    {
        m_pWeapon = std::make_unique<Weapon>(WeaponType::SWORD);
    }
    else if (weaponType == WeaponType::BOW)
    {
        m_pWeapon = std::make_unique<Weapon>(WeaponType::BOW);
    }
}