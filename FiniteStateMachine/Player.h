// Player.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Directions.h" // Assurez-vous d'inclure ceci

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

    // Nouvelle variable pour l'IA des fantômes
    Direction currentDirection;

public:
    Player();
    void update(float deltaTime, const Background& background);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    sf::FloatRect getGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    // Getters nécessaires pour l'IA
    Direction getDirection() const { return currentDirection; }

    // Helper pour avoir la position grille directement
    sf::Vector2i getGridPosition() const;
};