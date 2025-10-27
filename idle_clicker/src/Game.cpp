#include "../include/Game.h"
#include "../include/Button.h"
#include "../include/Upgrade.h"
#include "../include/Utils.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "Idle Clicker - Cookie Clicker")
    , m_cookies(0)
    , m_clickPower(1)
    , m_cookiesPerSecond(0)
    , m_clickAreaHovered(false)
    , m_productionTimer(0.0f)
{
    m_window.setFramerateLimit(60);

    // Load font
    if (!m_font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    // Setup click area (big cookie)
    m_clickArea.setRadius(80);
    m_clickArea.setOrigin({80, 80});
    m_clickArea.setPosition({200, 250});
    m_clickArea.setFillColor(sf::Color(210, 180, 140));  // Tan (cookie color)
    m_clickArea.setOutlineThickness(3);
    m_clickArea.setOutlineColor(sf::Color(139, 69, 19));  // Brown

    // Setup UI text
    m_titleText = std::make_unique<sf::Text>(m_font, "Cookie Clicker", 36);
    m_titleText->setPosition({250, 30});
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setStyle(sf::Text::Bold);

    m_cookiesText = std::make_unique<sf::Text>(m_font, "Cookies: 0", 24);
    m_cookiesText->setPosition({50, 100});
    m_cookiesText->setFillColor(sf::Color::Yellow);

    m_cpsText = std::make_unique<sf::Text>(m_font, "per second: 0", 18);
    m_cpsText->setPosition({50, 130});
    m_cpsText->setFillColor(sf::Color::Cyan);

    m_clickPowerText = std::make_unique<sf::Text>(m_font, "Click Power: 1", 18);
    m_clickPowerText->setPosition({50, 160});
    m_clickPowerText->setFillColor(sf::Color::Green);

    // Create upgrades
    m_upgrades.push_back(std::make_unique<Upgrade>("Cursor", 15, 1));
    m_upgrades.push_back(std::make_unique<Upgrade>("Grandma", 100, 5));
    m_upgrades.push_back(std::make_unique<Upgrade>("Farm", 500, 25));
    m_upgrades.push_back(std::make_unique<Upgrade>("Mine", 3000, 150));
    m_upgrades.push_back(std::make_unique<Upgrade>("Factory", 10000, 500));

    // Create upgrade buttons
    float yPos = 100;
    for (size_t i = 0; i < m_upgrades.size(); i++) {
        auto button = std::make_unique<Button>(
            sf::Vector2f(450, yPos),
            sf::Vector2f(330, 70),
            m_upgrades[i]->getName() + " - Cost: " + formatNumber(m_upgrades[i]->getCost()),
            m_font
        );
        m_upgradeButtons.push_back(std::move(button));
        yPos += 80;
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
                handleClick(sf::Mouse::getPosition(m_window));
            }
        }
    }

    handleMouseMove(sf::Mouse::getPosition(m_window));
}

void Game::update(float deltaTime) {
    // Auto production
    m_productionTimer += deltaTime;
    if (m_productionTimer >= 1.0f) {
        m_cookies += m_cookiesPerSecond;
        m_productionTimer = 0.0f;
    }

    // Update button states
    for (size_t i = 0; i < m_upgradeButtons.size(); i++) {
        bool canAfford = m_cookies >= m_upgrades[i]->getCost();
        m_upgradeButtons[i]->setEnabled(canAfford);
    }

    updateUI();
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 40));

    // Draw click area
    m_window.draw(m_clickArea);

    // Draw UI
    m_window.draw(*m_titleText);
    m_window.draw(*m_cookiesText);
    m_window.draw(*m_cpsText);
    m_window.draw(*m_clickPowerText);

    // Draw upgrade buttons
    for (const auto& button : m_upgradeButtons) {
        button->draw(m_window);
    }

    m_window.display();
}

void Game::handleClick(sf::Vector2i mousePos) {
    // Check click area
    sf::Vector2f mousePosF(mousePos);
    if (m_clickArea.getGlobalBounds().contains(mousePosF)) {
        m_cookies += m_clickPower;

        // Visual feedback - make cookie bigger briefly
        m_clickArea.setScale({1.1f, 1.1f});
        return;
    }

    // Check upgrade buttons
    for (size_t i = 0; i < m_upgradeButtons.size(); i++) {
        if (m_upgradeButtons[i]->isClicked(mousePos)) {
            tryPurchaseUpgrade(i);
            break;
        }
    }
}

void Game::handleMouseMove(sf::Vector2i mousePos) {
    // Update click area hover
    sf::Vector2f mousePosF(mousePos);
    bool wasHovered = m_clickAreaHovered;
    m_clickAreaHovered = m_clickArea.getGlobalBounds().contains(mousePosF);

    if (m_clickAreaHovered && !wasHovered) {
        m_clickArea.setScale({1.05f, 1.05f});
    } else if (!m_clickAreaHovered && wasHovered) {
        m_clickArea.setScale({1.0f, 1.0f});
    }

    // Update button hover states
    for (auto& button : m_upgradeButtons) {
        button->update(mousePos);
    }
}

void Game::tryPurchaseUpgrade(int index) {
    if (index < 0 || index >= static_cast<int>(m_upgrades.size())) return;

    auto& upgrade = m_upgrades[index];

    if (m_cookies >= upgrade->getCost()) {
        m_cookies -= upgrade->getCost();
        upgrade->purchase();

        // Recalculate cookies per second
        m_cookiesPerSecond = 0;
        for (const auto& upg : m_upgrades) {
            m_cookiesPerSecond += upg->getProduction();
        }

        std::cout << "Purchased " << upgrade->getName()
                  << " Level " << upgrade->getLevel() << "\n";
    }
}

void Game::updateUI() {
    m_cookiesText->setString("Cookies: " + formatNumber(m_cookies));
    m_cpsText->setString("per second: " + formatNumber(m_cookiesPerSecond));
    m_clickPowerText->setString("Click Power: " + formatNumber(m_clickPower));

    // Update upgrade buttons
    for (size_t i = 0; i < m_upgradeButtons.size(); i++) {
        std::string text = m_upgrades[i]->getName() + " (Lv." + std::to_string(m_upgrades[i]->getLevel()) + ")\n";
        text += "Cost: " + formatNumber(m_upgrades[i]->getCost());
        text += " | +" + formatNumber(m_upgrades[i]->getProduction() > 0 ?
                                       m_upgrades[i]->getProduction() / m_upgrades[i]->getLevel() :
                                       m_upgrades[i]->getProduction()) + "/s";
        m_upgradeButtons[i]->setText(text);
    }
}
