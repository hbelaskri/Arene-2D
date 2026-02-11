#pragma once
#include <SFML/System/Vector2.hpp>
#include "../Directions.h"

class IGhostEntity;

enum class GlobalGhostMode { Chase, Scatter, Frightened };

struct GhostContext
{
    IGhostEntity* ghost;

    GlobalGhostMode currentGlobalMode;
    float timeInCurrentState;

    sf::Vector2i targetGridPos; 
    Direction currentDirection; 

    int ghostID; 
};