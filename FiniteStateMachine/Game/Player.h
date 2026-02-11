#pragma once
#include <SFML/Graphics.hpp>
#include "Directions.h" 

class Background;

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    int currentFrame;
    float frameTime;
    float animationSpeed;
    sf::Vector2i frameSize;

    Direction currentDirection;

public:
    Player();
    void update(float deltaTime, const Background& background);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    sf::FloatRect getGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    Direction getDirection() const { return currentDirection; }

    sf::Vector2i getGridPosition() const;
};