#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Navigation.h"
#include "Background.h"

class Collectibles {
public:
    struct Item {
        sf::Sprite sprite;
        bool isActive = true;
        sf::Vector2i gridPos;

        Item(const sf::Texture& texture) : sprite(texture) {}
    };

    std::vector<Item> powerPellets;
    std::vector<Item> coins;

    Collectibles(const Background& bg);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& playerBounds);
    bool allCollected() const;
    void reset();

private:
    sf::Texture coinTexture;
    sf::Texture bigCoinTexture;

    int currentFrame;
    float frameTime;
    float animationSpeed;

    sf::Vector2i coinFrameSize;
    sf::Vector2i bigCoinFrameSize;

    void setupSprite(sf::Sprite& sprite, sf::Vector2i& frameSize, sf::Vector2i pos, float scaleFactor);
};