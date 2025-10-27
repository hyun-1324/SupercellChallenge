#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Player;
class Enemy;
class Bullet;

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    void handleMouseClick(sf::Vector2f mousePos);
    void spawnEnemy();
    void updateEnemies(float deltaTime);
    void updateBullets(float deltaTime);
    void checkCollisions();
    void updateUI();

    sf::RenderWindow m_window;
    std::unique_ptr<Player> m_player;
    std::vector<std::unique_ptr<Bullet>> m_bullets;
    std::vector<std::unique_ptr<Enemy>> m_enemies;

    sf::Clock m_deltaClock;
    sf::Font m_font;
    std::unique_ptr<sf::Text> m_scoreText;
    std::unique_ptr<sf::Text> m_enemyCountText;
    std::unique_ptr<sf::Text> m_controlsText;

    int m_score;
    float m_enemySpawnTimer;
    float m_enemySpawnInterval;

    sf::Vector2f m_mousePos;
};
