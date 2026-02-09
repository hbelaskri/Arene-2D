#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Navigation.h"
#include "Background.h"

class Collectibles {
public:
    struct PowerPellet {
        sf::CircleShape shape;
        bool isActive = true;
    };

    std::vector<PowerPellet> pellets;

    Collectibles() {
        std::vector<sf::Vector2i> cornerPositions = {
            {1, 1},   
            {28, 1},  
            {1, 18},  
            {28, 18}  
        };

        for (const auto& pos : cornerPositions) {
            PowerPellet pellet;
            
            float radius = 8.0f;
            pellet.shape.setRadius(radius);
            pellet.shape.setFillColor(sf::Color(148, 0, 211));
            
            pellet.shape.setOrigin({radius, radius}); 

            pellet.shape.setPosition(Navigation::GridToWorld(pos));

            pellets.push_back(pellet);
        }
    }

    bool checkCollision(const sf::FloatRect& playerBounds) {
        bool eaten = false;
        for (auto& pellet : pellets) {
            if (pellet.isActive) {
                if (pellet.shape.getGlobalBounds().findIntersection(playerBounds)) {
                    pellet.isActive = false; 
                    eaten = true;
                }
            }
        }
        return eaten;
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& pellet : pellets) {
            if (pellet.isActive) {
                window.draw(pellet.shape);
            }
        }
    }
    
    void reset() {
        for (auto& pellet : pellets) pellet.isActive = true;
    }
};