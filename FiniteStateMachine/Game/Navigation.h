#pragma once
#include <SFML/Graphics.hpp>
#include "Background.h"
#include "Directions.h"
#include <vector>

class Navigation {
public:
    static sf::Vector2i WorldToGrid(sf::Vector2f position);

    static sf::Vector2f GridToWorld(sf::Vector2i gridPos);

    static bool IsWalkable(int x, int y, const Background& bg);

    static Direction GetNextGhostDirection(
        sf::Vector2i currentGridPos,
        Direction currentDir,
        sf::Vector2i targetGridPos,
        const Background& bg
    );

    static float GetDistanceSq(sf::Vector2i a, sf::Vector2i b);
};