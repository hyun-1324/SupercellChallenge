#include "../include/Game.h"
#include "../include/Grid.h"
#include "../include/Item.h"
#include <iostream>
#include <random>
#include <cmath>

Game::Game()
    : m_window(sf::VideoMode({640, 720}), "Merge Game - SFML")
    , m_score(0)
    , m_draggedItem(nullptr)
    , m_spawnTimer(0.0f)
    , m_spawnInterval(3.0f)
{
    m_window.setFramerateLimit(60);

    // Create 8x8 grid (80px cells = 640px total)
    m_grid = std::make_unique<Grid>(8, 8, 80);

    // Load font
    if (!m_font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    // Setup UI
    m_scoreText = std::make_unique<sf::Text>(m_font, "Score: 0", 24);
    m_scoreText->setPosition({10, 650});
    m_scoreText->setFillColor(sf::Color::White);

    m_infoText = std::make_unique<sf::Text>(m_font, "Drag same numbers to merge!", 20);
    m_infoText->setPosition({10, 680});
    m_infoText->setFillColor(sf::Color::Cyan);

    // Spawn initial items
    for (int i = 0; i < 5; i++) {
        spawnRandomItem();
    }
}

Game::~Game() {}

void Game::run() {
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
                handleMousePress(sf::Mouse::getPosition(m_window));
            }
        }

        if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Left) {
                handleMouseRelease(sf::Mouse::getPosition(m_window));
            }
        }
    }

    // Handle mouse move
    if (m_draggedItem) {
        handleMouseMove(sf::Mouse::getPosition(m_window));
    }
}

void Game::update(float deltaTime) {
    // Auto-spawn items
    m_spawnTimer += deltaTime;
    if (m_spawnTimer >= m_spawnInterval && hasEmptyCell()) {
        spawnRandomItem();
        m_spawnTimer = 0;
    }

    updateUI();
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    m_grid->draw(m_window);

    // Draw items
    for (const auto& item : m_items) {
        if (!item->isDragging()) {
            item->draw(m_window);
        }
    }

    // Draw dragged item on top
    if (m_draggedItem) {
        m_draggedItem->draw(m_window);
    }

    m_window.draw(*m_scoreText);
    m_window.draw(*m_infoText);

    m_window.display();
}

void Game::handleMousePress(sf::Vector2i mousePos) {
    sf::Vector2f worldPos(mousePos);

    // Find item at click position (distance-based for better detection)
    for (auto& item : m_items) {
        sf::Vector2f itemPos = item->getPosition();
        sf::Vector2f diff = worldPos - itemPos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        // Click within 30 pixels (half of 60px item size)
        if (distance < 30.0f) {
            m_draggedItem = item.get();
            m_draggedItem->setDragging(true);
            m_dragStartGridPos = item->getGridPosition();
            m_dragOffset = worldPos - item->getPosition();

            // Remove from grid temporarily
            m_grid->removeItem(m_dragStartGridPos.x, m_dragStartGridPos.y);
            break;
        }
    }
}

void Game::handleMouseRelease(sf::Vector2i mousePos) {
    if (!m_draggedItem) return;

    sf::Vector2i gridPos = m_grid->worldToGrid(sf::Vector2f(mousePos));

    // Check if valid drop position
    if (m_grid->isValidPosition(gridPos.x, gridPos.y)) {
        Item* targetItem = m_grid->getItem(gridPos.x, gridPos.y);

        // Try to merge
        if (targetItem && targetItem->getLevel() == m_draggedItem->getLevel()) {
            // Merge!
            int newLevel = targetItem->getLevel() + 1;
            m_score += newLevel * 10;

            // Remove both items
            m_grid->removeItem(gridPos.x, gridPos.y);

            auto it = m_items.begin();
            while (it != m_items.end()) {
                if (it->get() == m_draggedItem || it->get() == targetItem) {
                    it = m_items.erase(it);
                } else {
                    ++it;
                }
            }

            // Create new merged item
            if (newLevel <= 7) {
                sf::Vector2f worldPos = m_grid->gridToWorld(gridPos.x, gridPos.y);
                auto newItem = std::make_unique<Item>(newLevel, worldPos);
                m_grid->setItem(gridPos.x, gridPos.y, newItem.get());
                m_items.push_back(std::move(newItem));
            }

            m_draggedItem = nullptr;
            return;
        }

        // Place in empty cell
        if (m_grid->isEmpty(gridPos.x, gridPos.y)) {
            sf::Vector2f worldPos = m_grid->gridToWorld(gridPos.x, gridPos.y);
            m_draggedItem->setPosition(worldPos);
            m_grid->setItem(gridPos.x, gridPos.y, m_draggedItem);
            m_draggedItem->setDragging(false);
            m_draggedItem = nullptr;
            return;
        }
    }

    // Invalid drop - return to original position
    sf::Vector2f worldPos = m_grid->gridToWorld(m_dragStartGridPos.x, m_dragStartGridPos.y);
    m_draggedItem->setPosition(worldPos);
    m_grid->setItem(m_dragStartGridPos.x, m_dragStartGridPos.y, m_draggedItem);
    m_draggedItem->setDragging(false);
    m_draggedItem = nullptr;
}

void Game::handleMouseMove(sf::Vector2i mousePos) {
    if (m_draggedItem) {
        sf::Vector2f worldPos(mousePos);
        m_draggedItem->setPosition(worldPos - m_dragOffset);
    }
}

void Game::spawnRandomItem() {
    if (!hasEmptyCell()) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> levelDist(1, 2);  // Spawn level 1 or 2

    // Find random empty cell
    std::vector<sf::Vector2i> emptyCells;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (m_grid->isEmpty(x, y)) {
                emptyCells.push_back({x, y});
            }
        }
    }

    if (emptyCells.empty()) return;

    std::uniform_int_distribution<> cellDist(0, emptyCells.size() - 1);
    sf::Vector2i gridPos = emptyCells[cellDist(gen)];

    int level = levelDist(gen);
    sf::Vector2f worldPos = m_grid->gridToWorld(gridPos.x, gridPos.y);

    auto item = std::make_unique<Item>(level, worldPos);
    m_grid->setItem(gridPos.x, gridPos.y, item.get());
    m_items.push_back(std::move(item));
}

bool Game::hasEmptyCell() const {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (m_grid->isEmpty(x, y)) {
                return true;
            }
        }
    }
    return false;
}

void Game::updateUI() {
    m_scoreText->setString("Score: " + std::to_string(m_score));
}
