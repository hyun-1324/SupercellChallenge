#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <functional>
#include "Constants.h"
#include "Upgrade.h"
#include "InputHandler.h"
#include <random>
#include <algorithm>

class Player;
class GameInput;
class Vampire;

namespace sf { class Clock; }

class Game : public sf::Drawable
{
public:
    
    enum class State
    {
        WAITING,
        ACTIVE,
    };
    
    Game();
    ~Game();
    
    bool initialise();
    void update(float deltaTime);
    void resetLevel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
    State getState() const { return m_state; }
	GameInput* getGameInput() { return m_pGameInput.get(); }
    
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);

    Player* getPlayer() const;
    sf::Texture* getPlayerTexture() { return &m_playerTexture; }
    sf::Texture* getVampireTexture() { return &m_vampTexture; }
	bool upgradePlayer(sf::RenderWindow& window);

    void vampireSpawner(float deltaTime);

	const sf::Font* getFont() const;
	void pauseGame(enum ePauseState pauseState);
	float getTime() { return old_time + m_pClock->getElapsedTime().asSeconds(); }



private:
    std::unique_ptr<Player> m_pPlayer;

    std::vector<std::unique_ptr<Vampire>> m_pVampires;

    State m_state;
    std::unique_ptr<sf::Clock> m_pClock;
    std::unique_ptr<GameInput> m_pGameInput;

    float m_vampireCooldown = 0.0f;
    float m_nextVampireCooldown = 2.0f;
    int m_spawnCount = 0;
	float old_time = 0.0f;
    
    sf::Font m_font;
    sf::Texture m_vampTexture;
    sf::Texture m_playerTexture;

	std::vector<Upgrade> m_upgrades;
};
