#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Background
{
public:

    Background();

    bool load(const std::string& texturepath);
    void draw(sf::RenderWindow& Window) const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
};