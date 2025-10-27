#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Button;
class Upgrade;

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    void handleClick(sf::Vector2i mousePos);
    void handleMouseMove(sf::Vector2i mousePos);
    void tryPurchaseUpgrade(int index);
    void updateUI();

    sf::RenderWindow m_window;
    sf::Font m_font;

    // Click area
    sf::CircleShape m_clickArea;
    bool m_clickAreaHovered;

    // Resources
    long long m_cookies;
    long long m_clickPower;
    long long m_cookiesPerSecond;

    // UI Text
    std::unique_ptr<sf::Text> m_cookiesText;
    std::unique_ptr<sf::Text> m_cpsText;
    std::unique_ptr<sf::Text> m_clickPowerText;
    std::unique_ptr<sf::Text> m_titleText;

    // Upgrades
    std::vector<std::unique_ptr<Upgrade>> m_upgrades;
    std::vector<std::unique_ptr<Button>> m_upgradeButtons;

    sf::Clock m_deltaClock;
    float m_productionTimer;
};
