#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "EndScreen.h"

enum class GameState {
    Menu,
    Options,
    Game,
    Pause, 
    End
};

class MenuManager {
public:
    explicit MenuManager(sf::RenderWindow& window);

    bool handleEvent(const std::optional<sf::Event>& event, GameState& currentState, sf::RenderWindow& window);

    void update(GameState currentState);
    void draw(GameState currentState);

    void setupEndScreen(const std::string& message, sf::Color color);

private:
    sf::RenderWindow& m_window;
    MainMenu m_mainMenu;
    OptionsMenu m_optionsMenu;
    EndScreen m_endScreen;

    sf::Font m_font;
    sf::Text m_pauseText;
};