#include "../include/PathFinding.h"
#include "../include/Grid.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <memory>

std::vector<sf::Vector2i> PathFinding::findPath(Grid* grid, sf::Vector2i start, sf::Vector2i end) {
    std::vector<std::unique_ptr<Node>> openList;
    std::vector<std::unique_ptr<Node>> closedList;

    auto startNode = std::make_unique<Node>(start);
    openList.push_back(std::move(startNode));

    while (!openList.empty()) {
        // Find node with lowest f score
        auto current = std::min_element(openList.begin(), openList.end(),
            [](const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) {
                return a->f < b->f;
            });

        Node* currentNode = current->get();

        // Reached end
        if (currentNode->pos == end) {
            auto path = reconstructPath(currentNode);
            // unique_ptr automatically cleans up!
            return path;
        }

        // Move to closed list
        closedList.push_back(std::move(*current));
        openList.erase(current);

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
                [&](const std::unique_ptr<Node>& n) { return n->pos == neighborPos; });
            if (inClosed != closedList.end()) continue;

            int newG = currentNode->g + 1;

            // Check if in open list
            auto inOpen = std::find_if(openList.begin(), openList.end(),
                [&](const std::unique_ptr<Node>& n) { return n->pos == neighborPos; });

            if (inOpen == openList.end()) {
                auto neighbor = std::make_unique<Node>(neighborPos);
                neighbor->g = newG;
                neighbor->h = heuristic(neighborPos, end);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = currentNode;
                openList.push_back(std::move(neighbor));
            } else if (newG < (*inOpen)->g) {
                (*inOpen)->g = newG;
                (*inOpen)->f = (*inOpen)->g + (*inOpen)->h;
                (*inOpen)->parent = currentNode;
            }
        }
    }

    // No path found - unique_ptr automatically cleans up!
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
