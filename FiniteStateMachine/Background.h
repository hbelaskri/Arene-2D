#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Decoration {
    sf::IntRect textureRect;
    sf::Vector2f position;
    sf::Vector2f sizeInTiles; 
};

class Background {
public:
    Background();
    bool load(const std::string& texturePath);
    void draw(sf::RenderWindow& window) const;
    bool isWall(int gridX, int gridY) const;

private:
    sf::Texture texture;
    mutable sf::Sprite tileSprite;
    std::vector<Decoration> formes;
    int map[15][20];
};