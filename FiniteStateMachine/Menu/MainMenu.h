#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdint>

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


    sf::Texture m_pacmanTexture;
    sf::Sprite m_pacmanSprite;
    sf::Texture m_coinTexture;

    sf::Vector2i m_pacmanFrameSize;

    std::vector<sf::Sprite> m_pelletSprites;
    std::vector<bool> m_pelletActive;

    std::vector<sf::CircleShape> m_stars;
    sf::Clock m_animClock;
};