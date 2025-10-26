#pragma once

#include "Rectangle.h"
#include "PowerUp.h"

#include <memory>

#define POWER_BAR_OFFSET 15.0f

struct InputData;

class Game;
class Weapon;


enum eDirection
{
    LEFT,
    RIGHT
};

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}
    bool initialise();
    void move(InputData inputData, float deltaTime);
    void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }
    unsigned int getHitPoints() { return m_hitPoints; }
    void setHitPoints(unsigned int hp) { m_hitPoints = hp; }
    void beHit();
    void setMessage(std::string str, sf::Color colour);
    void setImmunity(float time) { m_immunity = time; }
    float getImmunity() { return m_immunity; }
    Weapon* getWeapon() { return m_pWeapon.get(); }
    void receivePowerUp(eType type);
    float getGarlicBreath() { return m_garlicBreath; }
private:
    bool    m_isDead = false;
    unsigned int m_hitPoints = 6;
    int id;

    //timers
    float m_immunity = 0.f;
    float m_invincible = 0.f;
    float m_messageTimer = 0.f;
    float m_superSpeedTimer = 0.f;
    float m_garlicBreath = 0.f;

    eDirection m_direction = LEFT;
    bool m_changedDirection = false;
    Game*   m_pGame;
    std::unique_ptr<Weapon> m_pWeapon;
    sf::Text message;
    sf::RectangleShape powerUpBar;
    sf::RectangleShape powerUpBackground;
    void drawHealthBar(sf::RenderTarget &target) const;
    void decrementTimers(float deltaTime);
    void handlePowerUpBar();
    bool powerUpActive();
};
