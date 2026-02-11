#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class EndScreen
{
public:
    enum class EndAction { None, Retry, Menu };

    explicit EndScreen(sf::RenderWindow& window);

    void setup(const std::string& message, sf::Color color);
    EndAction handleEvent(const sf::Event& event);
    void update();
    void draw();

private:
    sf::RenderWindow& m_window;
    sf::Font m_font;
    sf::Text m_messageText;
    sf::Text m_retryButton;
    sf::Text m_menuButton;

    sf::Clock m_animationClock;
    bool m_isVictory = false;

    sf::Texture m_ghostTexture;
    std::vector<sf::Sprite> m_visualGhosts;
    std::vector<sf::Vector2f> m_ghostDirections; 
};