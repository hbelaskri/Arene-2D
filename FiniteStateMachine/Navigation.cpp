#include "Navigation.h"
#include <limits>
#include <cmath>
#include <array>

sf::Vector2i Navigation::WorldToGrid(sf::Vector2f position) {
    int x = static_cast<int>((position.x - 25.f) / Background::TILE_SIZE);
    int y = static_cast<int>((position.y - 50.f) / Background::TILE_SIZE);
    return { x, y };
}

sf::Vector2f Navigation::GridToWorld(sf::Vector2i gridPos) {
    float x = gridPos.x * Background::TILE_SIZE + 25.f + (Background::TILE_SIZE / 2.f);
    float y = gridPos.y * Background::TILE_SIZE + 50.f + (Background::TILE_SIZE / 2.f);
    return { x, y };
}

bool Navigation::IsWalkable(int x, int y, const Background& bg) {
    if (x < 0 || x >= Background::MAP_WIDTH || y < 0 || y >= Background::MAP_HEIGHT)
        return false;
    return bg.getTile(x, y) == 0;
}

float Navigation::GetDistanceSq(sf::Vector2i a, sf::Vector2i b) {
    float dx = static_cast<float>(a.x - b.x);
    float dy = static_cast<float>(a.y - b.y);
    return dx * dx + dy * dy;
}

Direction Navigation::GetNextGhostDirection(
    sf::Vector2i currentGridPos,
    Direction currentDir,
    sf::Vector2i targetGridPos,
    const Background& bg)
{
    Direction bestDir = Direction::None;
    float minDistance = std::numeric_limits<float>::max();

    std::array<Direction, 4> directions = {
        Direction::Up,
        Direction::Left,
        Direction::Down,
        Direction::Right
    };

    for (Direction dir : directions) {
        if (dir == GetOpposite(currentDir)) continue;

        sf::Vector2i dirVec = DirToVec(dir);
        sf::Vector2i nextTile = currentGridPos + dirVec;

        if (IsWalkable(nextTile.x, nextTile.y, bg)) {

            float dist = GetDistanceSq(nextTile, targetGridPos);

            if (dist < minDistance) {
                minDistance = dist;
                bestDir = dir;
            }
        }
    }

    if (bestDir == Direction::None) {
        return GetOpposite(currentDir);
    }

    return bestDir;
}