#pragma once

#include "Rectangle.h"
#include "Weapon.h"
#include "Arrow.h"

#include <memory>

struct InputData;

class Game;
class Weapon;
class Arrow;

enum eDirection
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Player : public Rectangle
{
public:
    Player(Game* pGame, WeaponType weaponType);
    virtual ~Player() {}
    
    bool initialise();
    void move(InputData inputData, float deltaTime);
    void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }
    void setWeaponType(WeaponType weaponType);

    Weapon* getWeapon() { return m_pWeapon.get(); }
    const std::vector<std::unique_ptr<Arrow>>& getArrows() const { return m_arrows; }

private:
    void shootArrow();
    std::vector<std::unique_ptr<Arrow>> m_arrows;

    bool    m_isDead = false;
    eDirection m_direction = LEFT;
    Game*   m_pGame;
    std::unique_ptr<Weapon> m_pWeapon;
};
