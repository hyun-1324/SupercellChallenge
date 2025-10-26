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
#include "PowerUp.h"

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_vampireCooldown(2.0f),
    m_nextVampireCooldown(2.0f)
{
    m_pPlayers.push_back(std::make_unique<Player>(this));
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayers[0].get(), nullptr);
}

Game::~Game()
{
}

bool Game::initialise()
{
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

    //heart base png from pixabay user janeb13
    if (!m_heartFull.loadFromFile(ResourceManager::getFilePath("heart_filled.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_heartHalf.loadFromFile(ResourceManager::getFilePath("heart_half_filled.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_heartEmpty.loadFromFile(ResourceManager::getFilePath("heart.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

    if (!m_apple.loadFromFile(ResourceManager::getFilePath("apple.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

    //original music by yours truly
    if (!m_music.openFromFile(ResourceManager::getFilePath("vampire_sir_hiver_theme.ogg")))
    {
        std::cerr << "Unable to load music" << std::endl;
        return false;
    }
    m_music.play();
    m_music.setLoop(true);
    m_music.setVolume(70);



    resetLevel();
    return true;
}

void Game::resetLevel()
{
    m_pVampires.clear();
    m_pPowerUps.clear();
    m_pPlayers[0]->getWeapon()->setActive(false);
    m_pPlayers[0]->initialise();
    if (m_state == State::CONTROLS_2P || m_state == State::ACTIVE_2P)
    {
        if (m_pPlayers.size() == 1)
            m_pPlayers.push_back(std::make_unique<Player>(this));
        m_pPlayers[1]->getWeapon()->setActive(false);
        m_pPlayers[1]->initialise();
    }
    initPowerUps();
    m_pClock->restart();
}

void Game::initPowerUps()
{
    m_pPowerUps.push_back(std::make_unique<PowerUp>(this, sf::Color::Red));
    m_pPowerUps.push_back(std::make_unique<PowerUp>(this, sf::Color::Blue));
    m_pPowerUps.push_back(std::make_unique<PowerUp>(this, sf::Color::Yellow));
}

void Game::handlePowerUps(float deltaTime)
{
    static float timer = 0;
    timer += deltaTime;
    if (timer > 15.0f)
    {
        int n = std::rand() % 3;
        sf::Color colour = (n == 0) ? sf::Color::Red :
        (n == 1) ? sf::Color::Blue :
        sf::Color::Yellow;
        m_pPowerUps.push_back(std::make_unique<PowerUp>(this, colour));
        timer = 0;
    }
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num1))
                m_state = State::CONTROLS_1P;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num2))
                m_state = State::CONTROLS_2P;
        }
        break;

        case State::CONTROLS_1P:
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::B))
            {
                m_state = State::ACTIVE_1P;
                m_pClock->restart();
            }
        }
        break;

        case State::CONTROLS_2P:
        {
            if (m_pPlayers.size() == 1) {
                m_pPlayers.push_back(std::make_unique<Player>(this));
                m_pPlayers[1]->initialise();
            }
            m_pGameInput->addPlayer2(m_pPlayers[1].get());
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::B))
            {
                m_state = State::ACTIVE_2P;
                m_pClock->restart();
            }
        }
        break;
            
        case State::ACTIVE_1P:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayers[0]->update(deltaTime);
            handlePowerUps(deltaTime);
            vampireSpawner(deltaTime);
            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
            }
            for (size_t i = 0; i < m_pPowerUps.size(); i ++)
            {
                int stepped = m_pPowerUps[i]->steppedOn();
                if (stepped == 0)
                {
                    m_pPlayers[0]->receivePowerUp(m_pPowerUps[i]->getType());
                    std::swap(m_pPowerUps[i], m_pPowerUps.back());
                    m_pPowerUps.pop_back();
                    
                }
                else if (stepped == 1)
                {
                    m_pPlayers[1]->receivePowerUp(m_pPowerUps[i]->getType());
                    std::swap(m_pPowerUps[i], m_pPowerUps.back());
                    m_pPowerUps.pop_back();
                }
            }

            if (m_pPlayers[0]->isDead())
            {
                m_state = State::END;
            }
        }
        break;

        case State::ACTIVE_2P:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayers[0]->update(deltaTime);
            m_pPlayers[1]->update(deltaTime);
            handlePowerUps(deltaTime);
            vampireSpawner(deltaTime);
            for (auto& temp : m_pVampires)
                temp->update(deltaTime);
            for (size_t i = 0; i < m_pPowerUps.size(); i ++)
            {
                int stepped = m_pPowerUps[i]->steppedOn();
                if (stepped == 0)
                {
                    m_pPlayers[0]->receivePowerUp(m_pPowerUps[i]->getType());
                    std::swap(m_pPowerUps[i], m_pPowerUps.back());
                    m_pPowerUps.pop_back();
                    
                }
                else if (stepped == 1)
                {
                    m_pPlayers[1]->receivePowerUp(m_pPowerUps[i]->getType());
                    std::swap(m_pPowerUps[i], m_pPowerUps.back());
                    m_pPowerUps.pop_back();
                }
            }

            if (m_pPlayers[0]->isDead() || m_pPlayers[1]->isDead())
            {
                m_state = State::END;
            }
        }
        break;

        case State::END:
        {
            // std::cout << "Current state: END" << std::endl;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num1))
            {
                m_state = State::CONTROLS_1P;
                resetLevel();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num2))
            {
                m_state = State::CONTROLS_2P;
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
    static bool gameOver = false;
    static int endTime = 0;
    sf::RectangleShape dimScreen(sf::Vector2f(ScreenWidth, ScreenHeight));
    dimScreen.setFillColor(sf::Color(0, 0, 0, 150));

    // Draw players.
    m_pPlayers[0]->draw(target, states);
    if ((m_state == State::CONTROLS_2P || m_state == State::ACTIVE_2P) && m_pPlayers.size() == 2)
        m_pPlayers[1]->draw(target, states);
    //Draw powerups
    for (size_t i = 0; i < m_pPowerUps.size(); i++) 
        m_pPowerUps[i]->draw(target, states);
    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }
    //  Draw texts.
    if (m_state == State::WAITING)
    {
        gameOver = false;
        sf::Text title;
        sf::Text startText;
        title.setFont(m_font);
        startText.setFont(m_font);
        title.setCharacterSize(66);
        startText.setCharacterSize(48);
        title.setString("Welcome to Vampire Sir Hivers!");
        startText.setString("Game template by Supercell\nAdditional code by Finn Shields\nOriginal Music by Finn Shields\n\nOne player mode: press \'1\'\nTwo player mode: press \'2\'");
        title.setFillColor(sf::Color::White);
        startText.setFillColor(sf::Color::White);
        title.setPosition(80.0f, 80.0f);
        startText.setPosition(80.0f, 170.0f);
        target.draw(dimScreen);
        target.draw(title);
        target.draw(startText);
    }
    else if(m_state == State::CONTROLS_1P)
    {
        sf::Text controls;
        controls.setFont(m_font);
        controls.setCharacterSize(48);
        controls.setString("Control with the arrow keys, and use your\namazing spear with the space bar\n\nPress \'B\' to begin!");
        controls.setFillColor(sf::Color::White);
        controls.setPosition(80.0f, 170.0f);
        target.draw(dimScreen);
        target.draw(controls);
    }
    else if(m_state == State::CONTROLS_2P)
    {
        sf::Text controls;
        controls.setFont(m_font);
        controls.setCharacterSize(48);
        controls.setString("Player 1 controls with the WASD key, and attacks with space\nPlayer 2 uses the arrow keys and  \nPress \'B\' to begin!");
        controls.setFillColor(sf::Color::White);
        controls.setPosition(80.0f, 170.0f);
        target.draw(dimScreen);
        target.draw(controls);
    }
    else if (m_state == State::ACTIVE_1P)
    {
        sf::Text timerText;
        timerText.setFont(m_font);
        timerText.setFillColor(sf::Color::White);
        timerText.setStyle(sf::Text::Bold);
        timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);
    }
    else if (m_state == State::ACTIVE_2P)
    {
        sf::Text timerText;
        timerText.setFont(m_font);
        timerText.setFillColor(sf::Color::White);
        timerText.setStyle(sf::Text::Bold);
        timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);
    }
    else
    {
        if (gameOver == false) {
            endTime = (int) m_pClock->getElapsedTime().asSeconds();
            gameOver = true;
        }
        sf::Text endText;
        endText.setFont(m_font);
        endText.setCharacterSize(48);
        std::string content;
        if (m_pPlayers.size() == 1)
            content = "Game over!\nYou survived for " + std::to_string(endTime) + " seconds.";
        else
            content = (m_pPlayers[0]->isDead()) ? "Well done player 2, you win!" : "Well done player 1, you win!";
        content += "\nNice work!\n\nPlay again?\nOne player mode: press \'1\'\nTwo player mode: press \'2\'";
        endText.setString(content);
        endText.setFillColor(sf::Color::White);
        endText.setPosition(80.0f, 90.0f);
        sf::RectangleShape dimScreen(sf::Vector2f(ScreenWidth, ScreenHeight));
        dimScreen.setFillColor(sf::Color(0, 0, 0, 150));
        target.draw(dimScreen);
        target.draw(endText);
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

Player* Game::getPlayer(int id) const 
{
    return m_pPlayers[id].get();
}

sf::Vector2f Game::getPlayerPos()
{
    return m_pPlayers[0]->getPosition();
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

    if (std::abs(xMinDist) < std::abs(yMinDist))
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

bool Game::checkCollisionWithVampires(Rectangle *object)
{
    for (size_t i = 0; i < m_pVampires.size(); i++)
        if (object != m_pVampires[i].get() && object->collidesWith(m_pVampires[i].get()))
            return true;
    return false;
}