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
    Direction newDir = Direction::None;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) newDir = Direction::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) newDir = Direction::Down;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) newDir = Direction::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) newDir = Direction::Right;

    if (newDir != Direction::None) {

        sf::Vector2i gridPos = Navigation::WorldToGrid(sprite.getPosition());
        sf::Vector2f centerPos = Navigation::GridToWorld(gridPos);

        float tolerance = 10.0f;

        bool isPerpendicular = (currentDirection == Direction::Left || currentDirection == Direction::Right)
            ? (newDir == Direction::Up || newDir == Direction::Down)
            : (newDir == Direction::Left || newDir == Direction::Right);

        if (isPerpendicular) {
            float distX = std::abs(sprite.getPosition().x - centerPos.x);
            float distY = std::abs(sprite.getPosition().y - centerPos.y);

            if ((newDir == Direction::Up || newDir == Direction::Down) && distX < tolerance) {
                sprite.setPosition({ centerPos.x, sprite.getPosition().y });
                currentDirection = newDir;
            }
            else if ((newDir == Direction::Left || newDir == Direction::Right) && distY < tolerance) {
                sprite.setPosition({ sprite.getPosition().x, centerPos.y });
                currentDirection = newDir;
            }
        }
        else {
            currentDirection = newDir;
        }
    }

    if (currentDirection == Direction::Up) {
        movement.y -= 1.f;
        sprite.setRotation(sf::degrees(270.f));
        sprite.setScale({ std::abs(sprite.getScale().x), std::abs(sprite.getScale().y) });
    }
    else if (currentDirection == Direction::Down) {
        movement.y += 1.f;
        sprite.setRotation(sf::degrees(90.f));
        sprite.setScale({ std::abs(sprite.getScale().x), std::abs(sprite.getScale().y) });
    }
    else if (currentDirection == Direction::Left) {
        movement.x -= 1.f;
        sprite.setRotation(sf::degrees(0.f));
        sprite.setScale({ -std::abs(sprite.getScale().x), std::abs(sprite.getScale().y) });
    }
    else if (currentDirection == Direction::Right) {
        movement.x += 1.f;
        sprite.setRotation(sf::degrees(0.f));
        sprite.setScale({ std::abs(sprite.getScale().x), std::abs(sprite.getScale().y) });
    }

    if (movement.x != 0.f || movement.y != 0.f)
    {
        sf::Vector2f directionVec = movement * speed * deltaTime;
        sf::Vector2f nextPos = sprite.getPosition() + directionVec;

        float offset = (Background::TILE_SIZE * 0.45f) / 2.0f;

        bool canMove = !background.isWall(nextPos.x + offset, nextPos.y + offset) &&
            !background.isWall(nextPos.x - offset, nextPos.y + offset) &&
            !background.isWall(nextPos.x + offset, nextPos.y - offset) &&
            !background.isWall(nextPos.x - offset, nextPos.y - offset);

        if (canMove)
        {
            sprite.setPosition(nextPos);

            frameTime += deltaTime;
            if (frameTime >= animationSpeed)
            {
                frameTime = 0.f;
                currentFrame = (currentFrame + 1) % 8;
                sprite.setTextureRect(sf::IntRect({ currentFrame * frameSize.x, 0 }, { frameSize.x, frameSize.y }));
            }
        }
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
