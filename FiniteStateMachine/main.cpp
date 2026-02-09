#include <SFML/Graphics.hpp>
#include "Player.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "EndScreen.h"

enum class GameState { Menu, Options, Playing, GameOver, Victory };

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "PacMan");
    window.setFramerateLimit(60);

    Player player;
    MainMenu menu(window);
    OptionsMenu options(window);
    EndScreen endScreen(window); 

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
           
            else if (currentState == GameState::GameOver || currentState == GameState::Victory) {
                EndScreen::EndAction action = endScreen.handleEvent(*event);
                if (action == EndScreen::EndAction::Retry) {
            
                    currentState = GameState::Playing;
                }
                if (action == EndScreen::EndAction::Menu) {
                    currentState = GameState::Menu;
                }
            }

         
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::G) {
                    endScreen.setup("GAME OVER", sf::Color::Red);
                    currentState = GameState::GameOver;
                }
                if (keyPressed->code == sf::Keyboard::Key::V) {
                    endScreen.setup("VICTOIRE", sf::Color::Green);
                    currentState = GameState::Victory;
                }
            }
        }

        if (currentState == GameState::Menu) menu.update();
        else if (currentState == GameState::Options) options.update();
        else if (currentState == GameState::GameOver || currentState == GameState::Victory) endScreen.update();
        else player.update(deltaTime);

        window.clear(sf::Color::Black);

        if (currentState == GameState::Menu) menu.draw();
        else if (currentState == GameState::Options) options.draw();
        else if (currentState == GameState::GameOver || currentState == GameState::Victory) endScreen.draw();
        else {
            player.draw(window);
        }

        window.display();
    }

    return 0;
}