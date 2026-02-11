#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:
    Background();
    void draw(sf::RenderWindow& window) const;
    bool isWall(float worldX, float worldY) const;

    static const int MAP_WIDTH = 30;
    static const int MAP_HEIGHT = 20;
    static const float TILE_SIZE; 

    int getTile(int x, int y) const {
        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
            return map[y][x];
        }
        return 1; 
    }

private:
    int map[MAP_HEIGHT][MAP_WIDTH];
};