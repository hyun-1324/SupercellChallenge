#include "../include/Game.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Bullet.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "Top-Down Shooter - Class Based")
    , m_score(0)
    , m_enemySpawnTimer(0.0f)
    , m_enemySpawnInterval(2.0f)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    m_window.setFramerateLimit(60);

    // Initialize player
    m_player = std::make_unique<Player>();

    // Load font
    if (!m_font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    // Setup UI texts - SFML 3.0 requires font in constructor
    m_scoreText = std::make_unique<sf::Text>(m_font, "Score: 0", 24);
    m_scoreText->setPosition({10, 10});
    m_scoreText->setFillColor(sf::Color::White);

    m_enemyCountText = std::make_unique<sf::Text>(m_font, "Enemies: 0", 20);
    m_enemyCountText->setPosition({10, 50});
    m_enemyCountText->setFillColor(sf::Color::Yellow);

    m_controlsText = std::make_unique<sf::Text>(m_font, "WASD: Move | Mouse: Aim | Click: Shoot", 16);
    m_controlsText->setPosition({200, 570});
    m_controlsText->setFillColor(sf::Color(150, 150, 150));
}

Game::~Game() {
}

void Game::run() {
    while (m_window.isOpen()) {
        float deltaTime = m_deltaClock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    // Update mouse position
    m_mousePos = sf::Vector2f(sf::Mouse::getPosition(m_window));

    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        // Handle mouse click
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                handleMouseClick(m_mousePos);
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Update player
    m_player->handleInput(deltaTime);
    m_player->update(deltaTime);
    m_player->updateAimLine(m_mousePos);

    // Spawn enemies
    m_enemySpawnTimer += deltaTime;
    if (m_enemySpawnTimer >= m_enemySpawnInterval) {
        spawnEnemy();
        m_enemySpawnTimer = 0;
    }

    // Update game objects
    updateBullets(deltaTime);
    updateEnemies(deltaTime);

    // Check collisions
    checkCollisions();

    // Update UI
    updateUI();
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    // Draw game objects
    m_player->draw(m_window);

    for (const auto& bullet : m_bullets) {
        bullet->draw(m_window);
    }

    for (const auto& enemy : m_enemies) {
        enemy->draw(m_window);
    }

    // Draw UI
    m_window.draw(*m_scoreText);
    m_window.draw(*m_enemyCountText);
    m_window.draw(*m_controlsText);

    m_window.display();
}

void Game::handleMouseClick(sf::Vector2f mousePos) {
    if (!m_player->canShoot()) {
        return;
    }

    // Calculate direction
    sf::Vector2f playerPos = m_player->getPosition();
    sf::Vector2f direction = mousePos - playerPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0) {
        direction /= length;  // Normalize

        float bulletSpeed = 500.0f;
        m_bullets.push_back(
            std::make_unique<Bullet>(playerPos, direction * bulletSpeed)
        );

        m_player->resetShootCooldown();
    }
}

void Game::spawnEnemy() {
    // Spawn from random side of screen
    int side = std::rand() % 4;
    sf::Vector2f spawnPos;

    switch (side) {
        case 0: spawnPos = {static_cast<float>(std::rand() % 800), -20.f}; break;  // Top
        case 1: spawnPos = {static_cast<float>(std::rand() % 800), 620.f}; break;  // Bottom
        case 2: spawnPos = {-20.f, static_cast<float>(std::rand() % 600)}; break;  // Left
        case 3: spawnPos = {820.f, static_cast<float>(std::rand() % 600)}; break;  // Right
    }

    m_enemies.push_back(std::make_unique<Enemy>(spawnPos));
}

void Game::updateEnemies(float deltaTime) {
    sf::Vector2f playerPos = m_player->getPosition();

    for (auto& enemy : m_enemies) {
        enemy->update(deltaTime, playerPos);
    }

    // Remove dead enemies
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<Enemy>& e) { return e->isDead(); }),
        m_enemies.end()
    );
}

void Game::updateBullets(float deltaTime) {
    for (auto& bullet : m_bullets) {
        bullet->update(deltaTime);
    }

    // Remove off-screen bullets
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const std::unique_ptr<Bullet>& b) { return b->isOffScreen(); }),
        m_bullets.end()
    );
}

void Game::checkCollisions() {
    // Bullet vs Enemy collision
    for (auto bulletIt = m_bullets.begin(); bulletIt != m_bullets.end(); ) {
        bool bulletHit = false;

        for (auto& enemy : m_enemies) {
            if ((*bulletIt)->getBounds().findIntersection(enemy->getBounds()).has_value()) {
                enemy->takeDamage(1);

                if (enemy->isDead()) {
                    m_score += 10;
                }

                bulletHit = true;
                break;
            }
        }

        if (bulletHit) {
            bulletIt = m_bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    // Enemy vs Player collision
    for (const auto& enemy : m_enemies) {
        if (m_player->getBounds().findIntersection(enemy->getBounds()).has_value()) {
            // Push player back
            sf::Vector2f pushDir = m_player->getPosition() - enemy->getPosition();
            float pushLength = std::sqrt(pushDir.x * pushDir.x + pushDir.y * pushDir.y);
            if (pushLength > 0) {
                pushDir /= pushLength;
                m_player->pushBack(pushDir, 200.0f, m_deltaClock.getElapsedTime().asSeconds());
            }
        }
    }
}

void Game::updateUI() {
    m_scoreText->setString("Score: " + std::to_string(m_score));
    m_enemyCountText->setString("Enemies: " + std::to_string(m_enemies.size()));
}
