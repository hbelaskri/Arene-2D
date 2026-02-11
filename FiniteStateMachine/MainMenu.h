#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class MainMenu
{
public:
   
    enum class MenuAction { None, Play, Options };

    explicit MainMenu(sf::RenderWindow& window);

    MenuAction handleEvent(const sf::Event& event); 
    void update();
    void draw();

private:
    sf::RenderWindow& m_window;

    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_play;
    sf::Text m_quit;
    sf::Text m_options;
};