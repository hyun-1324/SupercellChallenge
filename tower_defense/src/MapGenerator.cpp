#include "../include/MapGenerator.h"
#include "../include/Grid.h"

void MapGenerator::generateMap(Grid* grid, MapType type) {
    switch (type) {
        case MapType::Straight:
            createStraightPath(grid);
            break;
        case MapType::LShaped:
            createLShapedPath(grid);
            break;
        case MapType::SShaped:
            createSShapedPath(grid);
            break;
        case MapType::Zigzag:
            createZigzagPath(grid);
            break;
        case MapType::Spiral:
            createSpiralPath(grid);
            break;
    }
}

// 직선 (현재 맵)
void MapGenerator::createStraightPath(Grid* grid) {
    // Start (0,5) -> End (15,5)
    grid->setTile(0, 5, TileType::Start);
    for (int x = 1; x < 15; x++) {
        grid->setTile(x, 5, TileType::Path);
    }
    grid->setTile(15, 5, TileType::End);
}

// L자 경로
void MapGenerator::createLShapedPath(Grid* grid) {
    // Start (0,2) -> 오른쪽 -> 아래 -> End (15,11)

    grid->setTile(0, 2, TileType::Start);

    // 가로 (오른쪽으로)
    for (int x = 1; x <= 12; x++) {
        grid->setTile(x, 2, TileType::Path);
    }

    // 세로 (아래로)
    for (int y = 3; y <= 10; y++) {
        grid->setTile(12, y, TileType::Path);
    }

    // 가로 (오른쪽으로)
    for (int x = 13; x <= 14; x++) {
        grid->setTile(x, 10, TileType::Path);
    }

    grid->setTile(15, 10, TileType::End);
}

// S자 경로
void MapGenerator::createSShapedPath(Grid* grid) {
    // Start (0,2) -> S자 -> End (15,9)

    grid->setTile(0, 2, TileType::Start);

    // 첫 번째 가로선 (오른쪽)
    for (int x = 1; x <= 10; x++) {
        grid->setTile(x, 2, TileType::Path);
    }

    // 첫 번째 세로선 (아래)
    for (int y = 3; y <= 6; y++) {
        grid->setTile(10, y, TileType::Path);
    }

    // 두 번째 가로선 (왼쪽)
    for (int x = 9; x >= 5; x--) {
        grid->setTile(x, 6, TileType::Path);
    }

    // 두 번째 세로선 (아래)
    for (int y = 7; y <= 9; y++) {
        grid->setTile(5, y, TileType::Path);
    }

    // 세 번째 가로선 (오른쪽)
    for (int x = 6; x <= 14; x++) {
        grid->setTile(x, 9, TileType::Path);
    }

    grid->setTile(15, 9, TileType::End);
}

// 지그재그 경로
void MapGenerator::createZigzagPath(Grid* grid) {
    // Start (0,1) -> 지그재그 -> End (15,10)

    grid->setTile(0, 1, TileType::Start);

    int y = 1;

    // 오른쪽 → 아래 → 오른쪽 → 아래 반복
    for (int segment = 0; segment < 3; segment++) {
        int startX = segment * 5;
        int endX = startX + 5;

        // 가로
        for (int x = startX + 1; x <= endX && x < 15; x++) {
            grid->setTile(x, y, TileType::Path);
        }

        // 세로 (아래로 3칸)
        if (segment < 2) {
            for (int dy = 1; dy <= 3; dy++) {
                grid->setTile(endX, y + dy, TileType::Path);
            }
            y += 3;
        }
    }

    // 마지막 구간
    for (int x = 11; x <= 14; x++) {
        grid->setTile(x, y, TileType::Path);
    }

    grid->setTile(15, y, TileType::End);
}

// 나선형 경로
void MapGenerator::createSpiralPath(Grid* grid) {
    // Start (0,5) -> 나선형 -> End (8,5)

    grid->setTile(0, 5, TileType::Start);

    // 바깥쪽 사각형 (시계방향)

    // 오른쪽
    for (int x = 1; x <= 13; x++) {
        grid->setTile(x, 5, TileType::Path);
    }

    // 위로
    for (int y = 4; y >= 2; y--) {
        grid->setTile(13, y, TileType::Path);
    }

    // 왼쪽
    for (int x = 12; x >= 3; x--) {
        grid->setTile(x, 2, TileType::Path);
    }

    // 아래로
    for (int y = 3; y <= 9; y++) {
        grid->setTile(3, y, TileType::Path);
    }

    // 오른쪽
    for (int x = 4; x <= 10; x++) {
        grid->setTile(x, 9, TileType::Path);
    }

    // 위로
    for (int y = 8; y >= 5; y--) {
        grid->setTile(10, y, TileType::Path);
    }

    // 왼쪽 (중앙으로)
    for (int x = 9; x >= 7; x--) {
        grid->setTile(x, 5, TileType::Path);
    }

    grid->setTile(7, 5, TileType::End);
}
