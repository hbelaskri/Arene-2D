#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class OptionsMenu
{
public:
    explicit OptionsMenu(sf::RenderWindow& window);

    int handleEvent(const sf::Event& event);
    void update();
    void draw();

    bool isSoundOn() const { return m_soundEnabled; }

private:
    sf::RenderWindow& m_window;
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_soundToggle;
    sf::Text m_backButton;

    bool m_soundEnabled = true;
};