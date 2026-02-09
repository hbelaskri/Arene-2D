#include <SFML/Graphics.hpp>
#include "Player.h"
#include "MainMenu.h"
#include "OptionsMenu.h"

enum class GameState { Menu, Options, Playing };

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "PacMan");
    window.setFramerateLimit(60);

    Player player;
    MainMenu menu(window);
    OptionsMenu options(window); 

    GameState currentState = GameState::Menu;
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == GameState::Menu) {
          
                MainMenu::MenuAction action = menu.handleEvent(*event);

                if (action == MainMenu::MenuAction::Play) currentState = GameState::Playing;
                if (action == MainMenu::MenuAction::Options) currentState = GameState::Options;
            }
            else if (currentState == GameState::Options) {
           
                if (options.handleEvent(*event) == 1) currentState = GameState::Menu;
            }
        }

 
        if (currentState == GameState::Menu) menu.update();
        else if (currentState == GameState::Options) options.update();
        else player.update(deltaTime);

        window.clear(sf::Color::Black);

        if (currentState == GameState::Menu) menu.draw();
        else if (currentState == GameState::Options) options.draw();
        else {
            player.draw(window);
        }

        window.display();
    }

    return 0;
}