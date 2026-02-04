#pragma once
#include <SFML/Graphics.hpp>

class Player 
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;

    int currentFrame;
    float frameTime;
    float animationSpeed;
    sf::Vector2i frameSize;

public:
    Player(); 
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};