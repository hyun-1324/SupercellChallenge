#include "../include/Game.h"
#include "../include/Grid.h"
#include "../include/PathFinding.h"
#include "../include/Tower.h"
#include "../include/Enemy.h"
#include "../include/MapGenerator.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "Tower Defense - SFML")
    , m_currentWave(0)
    , m_gold(200)
    , m_lives(20)
    , m_enemiesSpawned(0)
    , m_enemiesPerWave(5)
    , m_spawnTimer(0.0f)
    , m_spawnInterval(2.0f)
    , m_currentMapType(MapType::Straight)
{
    m_window.setFramerateLimit(60);

    // Create grid (16x12 tiles, 50px each = 800x600)
    m_grid = std::make_unique<Grid>(16, 12, 50);

    // Load font
    if (!m_font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    // Setup UI
    m_waveText = std::make_unique<sf::Text>(m_font, "Wave: 0", 20);
    m_waveText->setPosition({10, 10});
    m_waveText->setFillColor(sf::Color::White);

    m_goldText = std::make_unique<sf::Text>(m_font, "Gold: 200", 20);
    m_goldText->setPosition({10, 35});
    m_goldText->setFillColor(sf::Color::Yellow);

    m_livesText = std::make_unique<sf::Text>(m_font, "Lives: 20", 20);
    m_livesText->setPosition({10, 60});
    m_livesText->setFillColor(sf::Color::Red);

    m_infoText = std::make_unique<sf::Text>(m_font, "Click to place tower (Cost: 100)", 16);
    m_infoText->setPosition({200, 10});
    m_infoText->setFillColor(sf::Color::Cyan);

    m_mapText = std::make_unique<sf::Text>(m_font, "Map: Straight (1-5 to change)", 16);
    m_mapText->setPosition({10, 570});
    m_mapText->setFillColor(sf::Color::Green);

    // Load initial map
    loadMap(MapType::Straight);
}

Game::~Game() {}

void Game::run() {
    // Start first wave
    spawnWave();

    while (m_window.isOpen()) {
        float deltaTime = m_deltaClock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                handleMouseClick(sf::Mouse::getPosition(m_window));
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyPress(keyPressed->code);
        }
    }
}

void Game::update(float deltaTime) {
    // Spawn enemies
    if (m_enemiesSpawned < m_enemiesPerWave) {
        m_spawnTimer += deltaTime;
        if (m_spawnTimer >= m_spawnInterval) {
            if (!m_path.empty()) {
                m_enemies.push_back(std::make_unique<Enemy>(m_path, m_grid->getTileSize()));
                m_enemiesSpawned++;
            }
            m_spawnTimer = 0;
        }
    }

    // Check for next wave
    if (m_enemiesSpawned >= m_enemiesPerWave && m_enemies.empty()) {
        spawnWave();
    }

    updateEnemies(deltaTime);
    updateTowers(deltaTime);
    updateProjectiles();
    updateUI();

    // Check game over
    if (m_lives <= 0) {
        std::cout << "Game Over! Wave: " << m_currentWave << std::endl;
        m_window.close();
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    m_grid->draw(m_window);

    for (const auto& tower : m_towers) {
        tower->draw(m_window);
    }

    for (const auto& enemy : m_enemies) {
        enemy->draw(m_window);
    }

    m_window.draw(*m_waveText);
    m_window.draw(*m_goldText);
    m_window.draw(*m_livesText);
    m_window.draw(*m_infoText);
    m_window.draw(*m_mapText);

    m_window.display();
}

void Game::handleMouseClick(sf::Vector2i mousePos) {
    sf::Vector2i gridPos = m_grid->worldToGrid(sf::Vector2f(mousePos));

    if (m_grid->canPlaceTower(gridPos.x, gridPos.y)) {
        const int towerCost = 100;

        if (m_gold >= towerCost) {
            sf::Vector2f worldPos = m_grid->gridToWorld(gridPos.x, gridPos.y);
            m_towers.push_back(std::make_unique<Tower>(worldPos));
            m_grid->setTile(gridPos.x, gridPos.y, TileType::Tower);
            m_gold -= towerCost;
        }
    }
}

void Game::spawnWave() {
    m_currentWave++;
    m_enemiesPerWave = 5 + m_currentWave * 2;
    m_enemiesSpawned = 0;
    std::cout << "Wave " << m_currentWave << " - " << m_enemiesPerWave << " enemies\n";
}

void Game::updateEnemies(float deltaTime) {
    for (auto& enemy : m_enemies) {
        enemy->update(deltaTime);
    }

    // Remove dead or reached enemies
    for (auto it = m_enemies.begin(); it != m_enemies.end(); ) {
        if ((*it)->isDead()) {
            m_gold += 25;  // Reward for kill
            it = m_enemies.erase(it);
        } else if ((*it)->hasReachedEnd()) {
            m_lives--;
            it = m_enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updateTowers(float deltaTime) {
    for (auto& tower : m_towers) {
        tower->update(deltaTime, m_enemies);
    }
}

void Game::updateProjectiles() {
    // Handled in Tower::update
}

void Game::checkProjectileCollisions() {
    // Handled in Projectile::update
}

void Game::updateUI() {
    m_waveText->setString("Wave: " + std::to_string(m_currentWave));
    m_goldText->setString("Gold: " + std::to_string(m_gold));
    m_livesText->setString("Lives: " + std::to_string(m_lives));
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    MapType newMapType = m_currentMapType;

    switch (key) {
        case sf::Keyboard::Key::Num1:
            newMapType = MapType::Straight;
            break;
        case sf::Keyboard::Key::Num2:
            newMapType = MapType::LShaped;
            break;
        case sf::Keyboard::Key::Num3:
            newMapType = MapType::SShaped;
            break;
        case sf::Keyboard::Key::Num4:
            newMapType = MapType::Zigzag;
            break;
        case sf::Keyboard::Key::Num5:
            newMapType = MapType::Spiral;
            break;
        default:
            return;
    }

    if (newMapType != m_currentMapType) {
        loadMap(newMapType);
    }
}

void Game::loadMap(MapType type) {
    m_currentMapType = type;

    // Clear grid
    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 16; x++) {
            m_grid->setTile(x, y, TileType::Empty);
        }
    }

    // Generate new map
    MapGenerator::generateMap(m_grid.get(), type);

    // Find start and end positions
    m_startPos = {-1, -1};
    m_endPos = {-1, -1};

    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 16; x++) {
            if (m_grid->getTile(x, y) == TileType::Start) {
                m_startPos = {x, y};
            }
            if (m_grid->getTile(x, y) == TileType::End) {
                m_endPos = {x, y};
            }
        }
    }

    // Calculate path using A*
    m_path = PathFinding::findPath(m_grid.get(), m_startPos, m_endPos);

    // Clear existing game state
    m_towers.clear();
    m_enemies.clear();
    m_currentWave = 0;
    m_gold = 200;
    m_lives = 20;
    m_enemiesSpawned = 0;
    m_enemiesPerWave = 5;

    // Update map text
    std::string mapName;
    switch (type) {
        case MapType::Straight: mapName = "Straight"; break;
        case MapType::LShaped: mapName = "L-Shaped"; break;
        case MapType::SShaped: mapName = "S-Shaped"; break;
        case MapType::Zigzag: mapName = "Zigzag"; break;
        case MapType::Spiral: mapName = "Spiral"; break;
    }
    m_mapText->setString("Map: " + mapName + " (1-5 to change)");

    std::cout << "Loaded map: " << mapName << "\n";
}
