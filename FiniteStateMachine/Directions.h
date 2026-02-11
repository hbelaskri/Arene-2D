#pragma once
#include <SFML/System/Vector2.hpp>

enum class Direction {
    None,
    Up,
    Left,
    Down,
    Right
};

static sf::Vector2i DirToVec(Direction dir) {
    switch (dir) {
    case Direction::Up:    return { 0, -1 };
    case Direction::Left:  return { -1, 0 };
    case Direction::Down:  return { 0, 1 };
    case Direction::Right: return { 1, 0 };
    default: return { 0, 0 };
    }
}

static Direction GetOpposite(Direction dir) {
    switch (dir) {
    case Direction::Up:    return Direction::Down;
    case Direction::Left:  return Direction::Right;
    case Direction::Down:  return Direction::Up;
    case Direction::Right: return Direction::Left;
    default: return Direction::None;
    }
}