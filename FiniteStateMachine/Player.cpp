#include "Player.h"
#include <iostream>
#include <cmath>

Player::Player()
    : sprite(texture), speed(100.f), currentFrame(0), frameTime(0.f), animationSpeed(0.1f)
{
    if (!texture.loadFromFile("../Assets/pacmanPack/PacMan.png")) 
    {
        std::cerr << "Erreur de chargement !" << std::endl;
    }

    frameSize.x = texture.getSize().x / 8;
    frameSize.y = texture.getSize().y;

    sprite.setTexture(texture);


    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize.x, frameSize.y }));
    sprite.setOrigin({ frameSize.x / 2.f, frameSize.y / 2.f });
    sprite.setPosition({ 400.f, 300.f });


    sprite.setScale({ 1.f, 1.f });
}

void Player::update(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        movement.y -= 1.f;
        sprite.setRotation(sf::degrees(270.f)); 
        sprite.setScale({ 1.f, 1.f });         
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += 1.f;
        sprite.setRotation(sf::degrees(90.f));  
        sprite.setScale({ 1.f, 1.f });
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        movement.x -= 1.f;
        sprite.setRotation(sf::degrees(0.f));   
        sprite.setScale({ -1.f, 1.f });        
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += 1.f;
        sprite.setRotation(sf::degrees(0.f));  
        sprite.setScale({ 1.f, 1.f });        
    }

    if (movement.x != 0.f || movement.y != 0.f)
    {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        sprite.move((movement / length) * speed * deltaTime);

 
        frameTime += deltaTime;
        if (frameTime >= animationSpeed)
        {
            frameTime = 0.f;
            currentFrame = (currentFrame + 1) % 8;
            sprite.setTextureRect(sf::IntRect({ currentFrame * frameSize.x, 0 }, { frameSize.x, frameSize.y }));
        }
    }
    else
    {
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize.x, frameSize.y }));
    }
}
void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}