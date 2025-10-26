#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "Constants.h"

class Player;
class Game;
class GameInput;
class Vampire;
class Rectangle;
class PowerUp;

namespace sf { class Clock; }

class Game : public sf::Drawable
{
public:
    enum class State
    {
        WAITING,
        CONTROLS_1P,
        CONTROLS_2P,
        ACTIVE_1P,
        ACTIVE_2P,
        END
    };
    Game();
    ~Game();
    bool initialise();
    void update(float deltaTime);
    void resetLevel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    State getState() const { return m_state; }
    sf::Vector2f getPlayerPos();
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);
    Player* getPlayer(int id) const;
    sf::Font* getFont() { return &m_font; }
    sf::Texture* getPlayerTexture() { return &m_playerTexture; }
    sf::Texture* getVampireTexture() { return &m_vampTexture; }
    sf::Texture* getHalfHeart() { return &m_heartHalf; }
    sf::Texture* getFullHeart() { return &m_heartFull; }
    sf::Texture* getEmptyHeart() { return &m_heartEmpty; }
    sf::Texture* getApple() { return &m_apple; }
    void vampireSpawner(float deltaTime);
    bool checkCollisionWithVampires(Rectangle *object);
private:
    std::vector<std::unique_ptr<Player>> m_pPlayers;
    std::vector<std::unique_ptr<Vampire>> m_pVampires;
    std::vector<std::unique_ptr<PowerUp>> m_pPowerUps;
    State m_state;
    std::unique_ptr<sf::Clock> m_pClock;
    std::unique_ptr<GameInput> m_pGameInput;
    float m_vampireCooldown = 0.0f;
    float m_nextVampireCooldown = 2.0f;
    int m_spawnCount = 0;
    sf::Font m_font;
    sf::Texture m_vampTexture;
    sf::Texture m_playerTexture;
    sf::Texture m_heartFull;
    sf::Texture m_heartHalf;
    sf::Texture m_heartEmpty;
    sf::Texture m_apple;
    sf::Music m_music;
    void initPowerUps();
    void handlePowerUps(float deltaTime);
};
