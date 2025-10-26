#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Game.h"

GameInput::GameInput(Game* pGame, Player* pPlayer1, Player* pPlayer2) :
    m_pGame(pGame), m_pPlayer1(pPlayer1), m_pPlayer2(pPlayer2)
{
}

GameInput::~GameInput()
{
}

void GameInput::update(float deltaTime)
{
    if (m_inputDataP1.hasInputs())
        m_pPlayer1->move(m_inputDataP1, deltaTime);
    if (m_inputDataP1.m_space)
        m_pPlayer1->attack();
    if (m_pGame->getState() == Game::State::ACTIVE_2P)
    {
        if (m_inputDataP2.hasInputs())
            m_pPlayer2->move(m_inputDataP2, deltaTime);
        if (m_inputDataP2.m_space)
            m_pPlayer2->attack();
    }
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
    if (m_pGame->getState() == Game::State::ACTIVE_1P)
    {
        if (key == sf::Keyboard::Up)
            m_inputDataP1.m_movingUp = true;
        else if (key == sf::Keyboard::Down)
            m_inputDataP1.m_movingDown = true;
        else if (key == sf::Keyboard::Left)
            m_inputDataP1.m_movingLeft = true;
        else if (key == sf::Keyboard::Right)
            m_inputDataP1.m_movingRight = true;
        else if (key == sf::Keyboard::Space)
        {
            if (m_inputDataP1.m_spaceReleased)
                m_inputDataP1.m_space = true;
            m_inputDataP1.m_spaceReleased = false;
        }
    }
    else if (m_pGame->getState() == Game::State::ACTIVE_2P)
    {
        if (key == sf::Keyboard::W)
            m_inputDataP1.m_movingUp = true;
        else if (key == sf::Keyboard::S)
            m_inputDataP1.m_movingDown = true;
        else if (key == sf::Keyboard::A)
            m_inputDataP1.m_movingLeft = true;
        else if (key == sf::Keyboard::D)
            m_inputDataP1.m_movingRight = true;
        else if (key == sf::Keyboard::Space)
        {
            if (m_inputDataP1.m_spaceReleased)
                m_inputDataP1.m_space = true;
            m_inputDataP1.m_spaceReleased = false;
        }
        else if (key == sf::Keyboard::Up)
            m_inputDataP2.m_movingUp = true;
        else if (key == sf::Keyboard::Down)
            m_inputDataP2.m_movingDown = true;
        else if (key == sf::Keyboard::Left)
            m_inputDataP2.m_movingLeft = true;
        else if (key == sf::Keyboard::Right)
            m_inputDataP2.m_movingRight = true;
        else if (key == sf::Keyboard::Enter)
        {
            if (m_inputDataP2.m_spaceReleased)
                m_inputDataP2.m_space = true;
            m_inputDataP2.m_spaceReleased = false;
        }
    }
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
    if (m_pGame->getState() == Game::State::ACTIVE_1P)
    {
        if (key == sf::Keyboard::Up)
            m_inputDataP1.m_movingUp = false;
        else if (key == sf::Keyboard::Down)
            m_inputDataP1.m_movingDown = false;
        else if (key == sf::Keyboard::Left)
            m_inputDataP1.m_movingLeft = false;
        else if (key == sf::Keyboard::Right)
            m_inputDataP1.m_movingRight = false;
        else if (key == sf::Keyboard::Space)
        {
            m_inputDataP1.m_space = false;
            m_inputDataP1.m_spaceReleased = true;
        }
    }
    else if (m_pGame->getState() == Game::State::ACTIVE_2P)
    {
        if (key == sf::Keyboard::W)
            m_inputDataP1.m_movingUp = false;
        else if (key == sf::Keyboard::S)
            m_inputDataP1.m_movingDown = false;
        else if (key == sf::Keyboard::A)
            m_inputDataP1.m_movingLeft = false;
        else if (key == sf::Keyboard::D)
            m_inputDataP1.m_movingRight = false;
        else if (key == sf::Keyboard::Space)
        {
            m_inputDataP1.m_space = false;
            m_inputDataP1.m_spaceReleased = true;
        }
        if (key == sf::Keyboard::Up)
            m_inputDataP2.m_movingUp = false;
        else if (key == sf::Keyboard::Down)
            m_inputDataP2.m_movingDown = false;
        else if (key == sf::Keyboard::Left)
            m_inputDataP2.m_movingLeft = false;
        else if (key == sf::Keyboard::Right)
            m_inputDataP2.m_movingRight = false;
        else if (key == sf::Keyboard::Enter)
        {
            m_inputDataP2.m_space = false;
            m_inputDataP2.m_spaceReleased = true;
        }
    }
    
}
