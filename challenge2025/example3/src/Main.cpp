#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Game.h"
#include <vector>
#include <memory>
#include <iostream>

#include "ResourceManager.h"

void handleKeyPress(sf::RenderWindow& window, std::unique_ptr<Game>& pGame, sf::Keyboard::Key key, sf::Clock& clock);
void upgradeScreen(sf::RenderWindow& window, std::unique_ptr<Game>& pGame, sf::Clock& clock);

int main(int argc, char* argv[])
{
    // ResourceManager Must be Instantiated here -- DO NOT CHANGE
    ResourceManager::init(argv[0]);

    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Survive");
    
    std::unique_ptr<Game> pGame = std::make_unique<Game>();
    if (!pGame->initialise())
    {
        std::cerr << "Game Failed to initialise" << std::endl;
        return 1;
    }
    
    sf::Clock clock;
	int upgradeTime = 1;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    // "close requested" event: we close the window
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    handleKeyPress(window, pGame, event.key.code, clock);
                    break;
                case sf::Event::KeyReleased:
                    pGame->onKeyReleased(event.key.code);
                    break;
                default:
                    break;
            }
        }
        
        sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();
        pGame->update(elapsedTime.asSeconds());
        
        // clear the window with black color
        window.clear(sf::Color::Black);
        
        window.draw(*pGame.get());
        
        // end the current frame
        window.display();
		if ((int)pGame->getTime() > upgradeTime * 5)
		{
			upgradeScreen(window, pGame, clock);
			upgradeTime += 1;
		}
    }
    
    return 0;
}


void upgradeScreen(sf::RenderWindow& window, std::unique_ptr<Game>& pGame, sf::Clock& clock)
{
	static int upgradeAmount = 0;
	upgradeAmount++;
	if (upgradeAmount > 5)
	{
		return;
	}
	sf::Text upgradeText;
	upgradeText.setFont(*pGame->getFont());
	upgradeText.setFillColor(sf::Color::White);
	upgradeText.setStyle(sf::Text::Bold);
	upgradeText.setString("To select an upgrade, press the corresponding number key. 5 upgrades max!");
	upgradeText.setPosition(sf::Vector2f(400, 60));
	window.draw(upgradeText);
	window.display();
	pGame->pauseGame(ePauseState::PAUSED);
	std::cout << "Upgrade Screen" << std::endl;
	pGame->upgradePlayer(window);
	std::cout << "Upgrade Screen" << std::endl;
	clock.restart();
	pGame->pauseGame(ePauseState::UNPAUSED);
}

void handleKeyPress(sf::RenderWindow& window, std::unique_ptr<Game>& pGame, sf::Keyboard::Key key, sf::Clock& clock)
{
	if (key == sf::Keyboard::R)
	{
		pGame->resetLevel();
	}
	else if (key == sf::Keyboard::Escape)
	{
		window.close();
	}
	else if (key == sf::Keyboard::P)
	{
		pGame->onKeyReleased(sf::Keyboard::Left);
		pGame->onKeyReleased(sf::Keyboard::Right);
		pGame->onKeyReleased(sf::Keyboard::Up);
		pGame->onKeyReleased(sf::Keyboard::Down);
		sf::Text pauseText;
		pauseText.setFont(*pGame->getFont());
		pauseText.setFillColor(sf::Color::White);
		pauseText.setStyle(sf::Text::Bold);
		pauseText.setString("Paused");
		pauseText.setPosition(sf::Vector2f(400, 20));
		window.draw(pauseText);
		window.display();
		pGame->pauseGame(ePauseState::PAUSED);
		while (1 && window.isOpen())
		{
			sf::Event event;
			window.pollEvent(event);
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				clock.restart();
				pGame->pauseGame(ePauseState::UNPAUSED);
				break;
			}
			else if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				break;
			}
		}
	}
	else
	{
		pGame->onKeyPressed(key);
	}
}
