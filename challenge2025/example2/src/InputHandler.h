#pragma once

#include <SFML/Window/Keyboard.hpp>

class Player;
class Game;

struct InputData
{
    bool m_movingUp = false;
    bool m_movingDown = false;
    bool m_movingLeft = false;
    bool m_movingRight = false;
    bool m_space = false;
    bool m_spaceReleased = true;
    bool hasInputs() { return m_movingUp || m_movingDown || m_movingLeft || m_movingRight || m_space;}
};

class GameInput
{
public:
    GameInput(Game* pGame, Player* pPlayer1, Player* pPlayer2);
    ~GameInput();
    
    void update(float deltaTime);
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);
    void addPlayer2(Player *pPlayer2) { if (m_pPlayer2) { return; } m_pPlayer2 = pPlayer2; };
private:
    InputData m_inputDataP1;
    InputData m_inputDataP2;
    Game* m_pGame;
    Player* m_pPlayer1;
    Player* m_pPlayer2;
};
