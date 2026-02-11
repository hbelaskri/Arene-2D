#include "Player.h"
#include "Background.h"
#include "Navigation.h" 
#include <iostream>
#include <cmath>

Player::Player()
    : texture(),
    sprite(texture),
    speed(150.f),
    currentFrame(0),
    frameTime(0.f),
    animationSpeed(0.1f),
    currentDirection(Direction::Right) 
{
    if (!texture.loadFromFile("../Assets/pacmanPack/PacMan.png"))
    {
        std::cerr << "Erreur : Impossible de charger ../Assets/pacmanPack/PacMan.png" << std::endl;
    }

    frameSize.x = static_cast<int>(texture.getSize().x) / 8; 
    frameSize.y = static_cast<int>(texture.getSize().y);

    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameSize.x, frameSize.y)));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize.x) / 2.0f, static_cast<float>(frameSize.y) / 2.0f));

    float targetSize = Background::TILE_SIZE * 0.75f;
    float scaleX = targetSize / static_cast<float>(frameSize.x);
    float scaleY = targetSize / static_cast<float>(frameSize.y);
    sprite.setScale(sf::Vector2f(scaleX, scaleY));

    sprite.setPosition(sf::Vector2f(Background::TILE_SIZE * 14.5f, Background::TILE_SIZE * 11.5f));
}

sf::Vector2i Player::getGridPosition() const {
    return Navigation::WorldToGrid(sprite.getPosition());
}

void Player::update(float deltaTime, const Background& background)
{
    sf::Vector2f movement(0.f, 0.f);
    float currentScaleVal = std::abs(sprite.getScale().x);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        movement.y -= 1.f;
        currentDirection = Direction::Up;
        sprite.setRotation(sf::degrees(270.f));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += 1.f;
        currentDirection = Direction::Down;
        sprite.setRotation(sf::degrees(90.f));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        movement.x -= 1.f;
        currentDirection = Direction::Left;
        sprite.setScale(sf::Vector2f(-currentScaleVal, currentScaleVal)); 
        sprite.setRotation(sf::degrees(0.f));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += 1.f;
        currentDirection = Direction::Right;
        sprite.setScale(sf::Vector2f(currentScaleVal, currentScaleVal));
        sprite.setRotation(sf::degrees(0.f));
    }

    if (movement.x != 0.f || movement.y != 0.f)
    {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        sf::Vector2f directionVec = (movement / length) * speed * deltaTime;

        sf::Vector2f nextPos = sprite.getPosition() + directionVec;
        float offset = (Background::TILE_SIZE * 0.70f) / 2.0f;

        bool canMove = !background.isWall(nextPos.x + offset, nextPos.y + offset) &&
            !background.isWall(nextPos.x - offset, nextPos.y + offset) &&
            !background.isWall(nextPos.x + offset, nextPos.y - offset) &&
            !background.isWall(nextPos.x - offset, nextPos.y - offset);

        if (canMove)
        {
            sprite.setPosition(nextPos);
        }

        frameTime += deltaTime;
        if (frameTime >= animationSpeed)
        {
            frameTime = 0.f;
            currentFrame = (currentFrame + 1) % 8; 
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(currentFrame * frameSize.x, 0), sf::Vector2i(frameSize.x, frameSize.y)));
        }
    }
    else
    {
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameSize.x, frameSize.y)));
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}