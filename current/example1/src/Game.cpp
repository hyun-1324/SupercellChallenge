#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Rectangle.h"
#include "Vampire.h"
#include "BloodPools.h"

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_WeapType(WeaponType::SWORD),
    m_pPlayer(std::make_unique<Player>(this, m_WeapType)),
    m_vampireCooldown(2.0f),
    m_nextVampireCooldown(2.0f)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
    // Initialize sword text
    m_swordText.setFont(m_font);
    m_swordText.setString("SWORD");
    m_swordText.setFillColor(sf::Color::White);
    m_swordText.setPosition(sf::Vector2f((ScreenWidth - m_swordText.getLocalBounds().width) * 0.5, 120.0f));
    m_swordText.setStyle(sf::Text::Bold);

    // Initialize bow text
    m_bowText.setFont(m_font);
    m_bowText.setString("BOW");
    m_bowText.setFillColor(sf::Color::White);
    m_bowText.setPosition(sf::Vector2f((ScreenWidth - m_bowText.getLocalBounds().width) * 0.5, 160.0f));
    m_bowText.setStyle(sf::Text::Bold);
}

Game::~Game()
{
}

// void Game::initCursor()
// {
//     if (!this->cursor.loadFromSystem(sf::Cursor::Cross))
//     {
//         std::cerr << "Unable to load cursor" << std::endl;
//     }
//     this->window->setMouseCursor(this->cursor);
// }

bool Game::initialise()
{
    /*
        PAY ATTENTION HIVER!
            If you want to load any assets (fonts, textures) please use the pattern shown below
    */

    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_bloodPoolTexture.loadFromFile(ResourceManager::getFilePath("kanefecti1.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

    resetLevel();
    return true;
}

int Game::getPoints() const
{
    /* returns the current points*/
    return this->points;
}

void Game::setPoints(int points)
{
    /* sets the new points*/
    this->points = points;
}
void Game::startGame()
{
    /* starts the game */
    if (this->m_state == State::WAITING)
    {
        this->m_state = State::ACTIVE;
    }
}

void Game::resetLevel()
{
    m_pVampires.clear();

    m_pPlayer->initialise();
    m_pClock->restart();
}
bool Game::handleMouseClick(const sf::RenderWindow& window)
{
    if (m_state == State::WAITING)
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        if (m_swordText.getGlobalBounds().contains(worldPos))
        {
            setWeaponType(WeaponType::SWORD);
            return true;
        }
        else if (m_bowText.getGlobalBounds().contains(worldPos))
        {
            setWeaponType(WeaponType::BOW);
            return true;
        }
    }
    return false;
}

void Game::setWeaponType(WeaponType type)
{
    m_WeapType = type;
    m_pPlayer->setWeaponType(type);
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            m_pClock->restart();
        }
        break;
            
        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->update(deltaTime);

            vampireSpawner(deltaTime);
            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
            }

            if (m_pPlayer->isDead())
            {
                m_state = State::WAITING;
                resetLevel();
            }
        }
        break;
    }

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            continue;
        }
        i++;
    }
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //  Draw texts.
    if (m_state == State::WAITING)
    {
        // Starting Texts with weapon options
        sf::Text startText;
        startText.setFont(m_font);
        startText.setString("Choose a weapon: ");
        startText.setFillColor(sf::Color::White);
        startText.setPosition(sf::Vector2f((ScreenWidth - startText.getLocalBounds().getSize().x) * 0.5, 80.0f));
        startText.setStyle(sf::Text::Bold);
        target.draw(startText);
        target.draw(m_swordText);
        target.draw(m_bowText);
    }
    else
    {
        sf::Text timerText;
        timerText.setFont(m_font);
        timerText.setFillColor(sf::Color::White);
        timerText.setStyle(sf::Text::Bold);
        timerText.setString("Time: " + std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);

        sf::Text pointsText;
        pointsText.setFont(m_font);
        pointsText.setFillColor(sf::Color::Green);
        pointsText.setStyle(sf::Text::Italic);
        pointsText.setString("Points: " + std::to_string(this->points));
        pointsText.setPosition(sf::Vector2f((ScreenWidth - pointsText.getLocalBounds().getSize().x) * 0.5, 60));
        target.draw(pointsText);
    }

    // Draw player.
    m_pPlayer->draw(target, states);

    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }
}


void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

Player* Game::getPlayer() const 
{
    return m_pPlayer.get();
}

void Game::vampireSpawner(float deltaTime)
{
    if (m_vampireCooldown > 0.0f)
    {
        m_vampireCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    float distToRight = (float) ScreenWidth - randomXPos;
    float distToBottom = (float) ScreenHeight - randomYPos;

    float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
    float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

    if (abs(xMinDist) < abs(yMinDist))
        randomXPos += xMinDist;
    else
        randomYPos += yMinDist;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_spawnCount % 5 == 0)
    {
        m_nextVampireCooldown -= 0.1f;
    }
    m_vampireCooldown = m_nextVampireCooldown;
}