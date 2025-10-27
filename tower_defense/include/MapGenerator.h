#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Grid;

enum class MapType {
    Straight,    // 직선 (현재)
    LShaped,     // L자
    SShaped,     // S자
    Zigzag,      // 지그재그
    Spiral       // 나선형
};

class MapGenerator {
public:
    static void generateMap(Grid* grid, MapType type);

private:
    static void createStraightPath(Grid* grid);
    static void createLShapedPath(Grid* grid);
    static void createSShapedPath(Grid* grid);
    static void createZigzagPath(Grid* grid);
    static void createSpiralPath(Grid* grid);
};
