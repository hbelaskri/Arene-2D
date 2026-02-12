#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdint>

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

    sf::Texture m_pacmanTexture;
    sf::Sprite m_pacmanSprite;
    sf::Texture m_coinTexture;
    sf::Vector2i m_pacmanFrameSize;

    std::vector<sf::Sprite> m_pelletSprites;
    std::vector<bool> m_pelletActive;
    std::vector<sf::CircleShape> m_stars;
    sf::Clock m_animClock;
};
