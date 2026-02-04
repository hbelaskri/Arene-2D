#include "Background.h"
#include <iostream>

Background::Background() : sprite(texture)
{
}

bool Background::load(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur: impossible de charger " << texturePath << std::endl;
        return false;
    }

    sprite.setTexture(texture);


    sf::Vector2u size = texture.getSize();
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)size.x, (int)size.y }));

  
    sprite.setScale({ 800.f / size.x, 600.f / size.y });

    return true;
}

void Background::draw(sf::RenderWindow& window) const 
{
    window.draw(sprite);
}