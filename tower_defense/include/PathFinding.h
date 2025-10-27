#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Grid;

class PathFinding {
public:
    static std::vector<sf::Vector2i> findPath(
        Grid* grid,
        sf::Vector2i start,
        sf::Vector2i end
    );

private:
    struct Node {
        sf::Vector2i pos;
        int g;  // Cost from start
        int h;  // Heuristic to end
        int f;  // g + h
        Node* parent;

        Node(sf::Vector2i position)
            : pos(position), g(0), h(0), f(0), parent(nullptr) {}
    };

    static int heuristic(sf::Vector2i a, sf::Vector2i b);
    static std::vector<sf::Vector2i> reconstructPath(Node* endNode);
};
