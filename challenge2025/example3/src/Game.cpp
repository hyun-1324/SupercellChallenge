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

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_vampireCooldown(2.0f),
    m_nextVampireCooldown(2.0f),
	old_time(0.0f)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
	m_upgrades ={
		{"Health Upgrade", [](Player& player) {player.setHealth(player.getHealth() + 2); },},
		{"Speed Upgrade", [](Player& player) {player.setSpeed(player.getSpeed() + 10); }},
		{"Attack Speed Upgrade", [](Player& player) {player.setAttackSpeed(player.getAttackSpeed() - 0.15f); }},
		{"Whip Length Upgrade", [](Player& player) {player.setWhipLength(player.getWhipLength() + 20); }}
	};
}

Game::~Game()
{
}

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

    resetLevel();
    return true;
}

void Game::resetLevel()
{
	m_pVampires.clear();
	m_pPlayer->initialise();
	m_pPlayer->getWeapon()->reset();
	m_pClock->restart();
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (m_pClock->getElapsedTime().asSeconds() >= 3.f)
            {
                m_state = State::ACTIVE;
                m_pClock->restart();
            }
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
				this->m_nextVampireCooldown = 2.0f;
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
        sf::Text startText;
        startText.setFont(m_font);
        startText.setString("Game Start!");
        startText.setFillColor(sf::Color::White);
        startText.setPosition(80.0f, 80.0f);
        startText.setStyle(sf::Text::Bold);
        target.draw(startText);
    }
    else
    {
        sf::Text timerText;
        timerText.setFont(m_font);
        timerText.setFillColor(sf::Color::White);
        timerText.setStyle(sf::Text::Bold);
        timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds() + (int)old_time));
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);
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

    if (std::abs(xMinDist) < std::abs(yMinDist))
        randomXPos += xMinDist;
    else
        randomYPos += yMinDist;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_spawnCount % 5 == 0 || m_nextVampireCooldown < 0.5f)
    {
        m_nextVampireCooldown -= 0.1f;
    }
    m_vampireCooldown = m_nextVampireCooldown;
}

const sf::Font* Game::getFont() const
{
    return &m_font;
}

void Game::pauseGame(enum ePauseState pauseState)
{
	if (pauseState == ePauseState::PAUSED)
	{
		old_time += m_pClock->getElapsedTime().asSeconds();
	}
	else
	{
		m_pClock->restart();
	}
}

bool Game::upgradePlayer(sf::RenderWindow& window)
{
	std::vector<Upgrade> selectedUpgrades;
	std::sample(m_upgrades.begin(), m_upgrades.end(), std::back_inserter(selectedUpgrades), 3, std::mt19937{std::random_device{}()});

	for (int i = 0; i < selectedUpgrades.size(); i++)
	{
		sf::Text upgradeText;
		upgradeText.setFont(m_font);
		upgradeText.setFillColor(sf::Color::White);
		upgradeText.setStyle(sf::Text::Bold);
		upgradeText.setString(std::to_string(i + 1) + ": " + selectedUpgrades[i].description);
		upgradeText.setPosition(sf::Vector2f(400, 100 + (i * 30)));
		window.draw(upgradeText);
	}
	window.display();
	std::cout << "Upgrade Screen inside upgrade player function" << std::endl;

	onKeyReleased(sf::Keyboard::Key::Up);
	onKeyReleased(sf::Keyboard::Key::Down);
	onKeyReleased(sf::Keyboard::Key::Left);
	onKeyReleased(sf::Keyboard::Key::Right);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			std::cout << "Upgrade Screen inside upgrade player function while loop" << std::endl;
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					std::cout << "1" << std::endl;
					selectedUpgrades[0].apply(*m_pPlayer);
					return true;
				}
				else if (event.key.code == sf::Keyboard::Num2)
				{
					std::cout << "2" << std::endl;
					selectedUpgrades[1].apply(*m_pPlayer);
					return true;
				}
				else if (event.key.code == sf::Keyboard::Num3)
				{
					std::cout << "3" << std::endl;
					selectedUpgrades[2].apply(*m_pPlayer);
					return true;
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					std::cout << "Escape" << std::endl;
					window.close();
					return false;
				}
			}
			else if (event.type == sf::Event::Closed)
			{
				std::cout << "Closed" << std::endl;
				window.close();
				return false;
			}
		}
	}
	return false;
}
