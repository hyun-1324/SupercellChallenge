#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "MapGenerator.h"

class Grid;
class Tower;
class Enemy;

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    void handleMouseClick(sf::Vector2i mousePos);
    void handleKeyPress(sf::Keyboard::Key key);
    void loadMap(MapType type);
    void spawnWave();
    void updateEnemies(float deltaTime);
    void updateTowers(float deltaTime);
    void updateProjectiles();
    void checkProjectileCollisions();
    void updateUI();

    sf::RenderWindow m_window;
    std::unique_ptr<Grid> m_grid;
    std::vector<std::unique_ptr<Tower>> m_towers;
    std::vector<std::unique_ptr<Enemy>> m_enemies;

    sf::Clock m_deltaClock;
    sf::Font m_font;
    std::unique_ptr<sf::Text> m_waveText;
    std::unique_ptr<sf::Text> m_goldText;
    std::unique_ptr<sf::Text> m_livesText;
    std::unique_ptr<sf::Text> m_infoText;
    std::unique_ptr<sf::Text> m_mapText;

    int m_currentWave;
    int m_gold;
    int m_lives;
    int m_enemiesSpawned;
    int m_enemiesPerWave;
    float m_spawnTimer;
    float m_spawnInterval;

    std::vector<sf::Vector2i> m_path;  // Pre-calculated path
    MapType m_currentMapType;
    sf::Vector2i m_startPos;
    sf::Vector2i m_endPos;
};
