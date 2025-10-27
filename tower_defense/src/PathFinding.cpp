#include "../include/PathFinding.h"
#include "../include/Grid.h"
#include <algorithm>
#include <cmath>
#include <queue>

std::vector<sf::Vector2i> PathFinding::findPath(Grid* grid, sf::Vector2i start, sf::Vector2i end) {
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    Node* startNode = new Node(start);
    Node* endNode = new Node(end);

    openList.push_back(startNode);

    while (!openList.empty()) {
        // Find node with lowest f score
        auto current = std::min_element(openList.begin(), openList.end(),
            [](Node* a, Node* b) { return a->f < b->f; });

        Node* currentNode = *current;

        // Reached end
        if (currentNode->pos == end) {
            auto path = reconstructPath(currentNode);

            // Cleanup
            for (auto node : openList) delete node;
            for (auto node : closedList) delete node;
            delete endNode;

            return path;
        }

        openList.erase(current);
        closedList.push_back(currentNode);

        // Check neighbors (4-directional)
        std::vector<sf::Vector2i> neighbors = {
            {currentNode->pos.x + 1, currentNode->pos.y},
            {currentNode->pos.x - 1, currentNode->pos.y},
            {currentNode->pos.x, currentNode->pos.y + 1},
            {currentNode->pos.x, currentNode->pos.y - 1}
        };

        for (const auto& neighborPos : neighbors) {
            // Skip invalid or non-path tiles
            if (!grid->isValid(neighborPos.x, neighborPos.y)) continue;

            TileType tile = grid->getTile(neighborPos.x, neighborPos.y);
            if (tile != TileType::Path && tile != TileType::End) continue;

            // Check if in closed list
            auto inClosed = std::find_if(closedList.begin(), closedList.end(),
                [&](Node* n) { return n->pos == neighborPos; });
            if (inClosed != closedList.end()) continue;

            int newG = currentNode->g + 1;

            // Check if in open list
            auto inOpen = std::find_if(openList.begin(), openList.end(),
                [&](Node* n) { return n->pos == neighborPos; });

            if (inOpen == openList.end()) {
                Node* neighbor = new Node(neighborPos);
                neighbor->g = newG;
                neighbor->h = heuristic(neighborPos, end);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = currentNode;
                openList.push_back(neighbor);
            } else if (newG < (*inOpen)->g) {
                (*inOpen)->g = newG;
                (*inOpen)->f = (*inOpen)->g + (*inOpen)->h;
                (*inOpen)->parent = currentNode;
            }
        }
    }

    // No path found - cleanup
    for (auto node : openList) delete node;
    for (auto node : closedList) delete node;
    delete endNode;

    return {};  // Empty path
}

int PathFinding::heuristic(sf::Vector2i a, sf::Vector2i b) {
    // Manhattan distance
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<sf::Vector2i> PathFinding::reconstructPath(Node* endNode) {
    std::vector<sf::Vector2i> path;
    Node* current = endNode;

    while (current != nullptr) {
        path.push_back(current->pos);
        current = current->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}
