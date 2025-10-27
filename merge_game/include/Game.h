#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Grid;
class Item;

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    void handleMousePress(sf::Vector2i mousePos);
    void handleMouseRelease(sf::Vector2i mousePos);
    void handleMouseMove(sf::Vector2i mousePos);

    void tryMerge(int x, int y);
    void spawnRandomItem();
    bool hasEmptyCell() const;
    void updateUI();

    sf::RenderWindow m_window;
    std::unique_ptr<Grid> m_grid;
    std::vector<std::unique_ptr<Item>> m_items;

    sf::Clock m_deltaClock;
    sf::Font m_font;
    std::unique_ptr<sf::Text> m_scoreText;
    std::unique_ptr<sf::Text> m_infoText;

    int m_score;
    Item* m_draggedItem;
    sf::Vector2f m_dragOffset;
    sf::Vector2i m_dragStartGridPos;

    float m_spawnTimer;
    float m_spawnInterval;
};
