#include <SFML/Graphics.hpp>
#include "Player.h"


//int main() 
//{
//   /* Background background;
//    if (!background.load("../Assets/pacmanPack/Background.png"))
//    {
//        return -1; 
//    }*/
//
//    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "PacMan ");
//    window.setFramerateLimit(60);
//
//    Player player;
//
//    sf::Clock clock;
//
//    while (window.isOpen()) 
//    {
//        window.clear(sf::Color::Black);
//        player.draw(window);     
//
//        float deltaTime = clock.restart().asSeconds();
//
//        while (const std::optional event = window.pollEvent()) {
//            if (event->is<sf::Event::Closed>()) window.close();
//        }
//
//        player.update(deltaTime);
//        player.draw(window);
//        window.display();
//    }
//
//    return 0;
//}