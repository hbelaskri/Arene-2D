#include "OptionsMenu.h"
#include <cmath>

OptionsMenu::OptionsMenu(sf::RenderWindow& window)
    : m_window(window),
    m_title(m_font, ""), m_soundToggle(m_font, ""), m_backButton(m_font, ""),
    m_pacmanSprite(m_pacmanTexture)
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf")) {
        std::cerr << "Erreur : police non chargee\n";
    }

    m_pacmanTexture.loadFromFile("../Assets/pacmanPack/PacMan.png");
    m_coinTexture.loadFromFile("../Assets/pacmanPack/Coin.png");

    m_pacmanFrameSize.x = static_cast<int>(m_pacmanTexture.getSize().x) / 8;
    m_pacmanFrameSize.y = static_cast<int>(m_pacmanTexture.getSize().y);

    float centerX = m_window.getSize().x / 2.0f;

    m_title.setString("OPTIONS");
    m_title.setCharacterSize(50);
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect tB = m_title.getLocalBounds();
    m_title.setOrigin({ tB.size.x / 2.0f, tB.size.y / 2.0f });
    m_title.setPosition({ centerX, 100.f });

    m_soundToggle.setString("SON : ON");
    m_soundToggle.setCharacterSize(30);
    sf::FloatRect sB = m_soundToggle.getLocalBounds();
    m_soundToggle.setOrigin({ sB.size.x / 2.0f, sB.size.y / 2.0f });
    m_soundToggle.setPosition({ centerX, 280.f });

    m_backButton.setString("RETOUR");
    m_backButton.setCharacterSize(30);
    sf::FloatRect rB = m_backButton.getLocalBounds();
    m_backButton.setOrigin({ rB.size.x / 2.0f, rB.size.y / 2.0f });
    m_backButton.setPosition({ centerX, 380.f });

    for (int i = 0; i < 60; ++i) {
        sf::CircleShape star(1.5f);
        star.setPosition({ static_cast<float>(rand() % m_window.getSize().x),
                           static_cast<float>(rand() % m_window.getSize().y) });
        star.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(rand() % 180)));
        m_stars.push_back(star);
    }

    for (int i = 0; i < 15; ++i) {
        sf::Sprite s(m_coinTexture);
        s.setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));
        s.setOrigin({ 8.f, 8.f });
        s.setPosition({ i * 80.f, 185.f });
        m_pelletSprites.push_back(s);
        m_pelletActive.push_back(true);
    }

    m_pacmanSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), m_pacmanFrameSize));
    m_pacmanSprite.setOrigin({ static_cast<float>(m_pacmanFrameSize.x) / 2.0f, static_cast<float>(m_pacmanFrameSize.y) / 2.0f });
    m_pacmanSprite.setScale({ 2.0f, 2.0f });
    m_pacmanSprite.setPosition({ -50.f, 185.f });
}

int OptionsMenu::handleEvent(const sf::Event& event)
{
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseClick->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(mouseClick->position.x),
                static_cast<float>(mouseClick->position.y));

            if (m_backButton.getGlobalBounds().contains(mousePos)) return 1;

            if (m_soundToggle.getGlobalBounds().contains(mousePos)) {
                m_soundEnabled = !m_soundEnabled;
                m_soundToggle.setString(m_soundEnabled ? "SON : ON" : "SON : OFF");

                sf::FloatRect sB = m_soundToggle.getLocalBounds();
                m_soundToggle.setOrigin({ sB.size.x / 2.0f, sB.size.y / 2.0f });
                return 2;
            }
        }
    }
    return 0;
}

void OptionsMenu::update()
{
    sf::Vector2i mouseI = sf::Mouse::getPosition(m_window);
    sf::Vector2f mousePos(static_cast<float>(mouseI.x), static_cast<float>(mouseI.y));
    float time = m_animClock.getElapsedTime().asSeconds();

   
    int alphaVal = static_cast<int>(180 + 75 * std::sin(time * 4.0f));
    m_title.setFillColor(sf::Color(255, 255, 0, static_cast<uint8_t>(alphaVal)));

  
    int frame = static_cast<int>(time * 12.0f) % 8;
    m_pacmanSprite.setTextureRect(sf::IntRect(sf::Vector2i(frame * m_pacmanFrameSize.x, 0), m_pacmanFrameSize));
    m_pacmanSprite.move({ 180.f * 0.016f, 0.f });

    for (size_t i = 0; i < m_pelletSprites.size(); ++i) {
        if (m_pelletActive[i] && m_pacmanSprite.getGlobalBounds().contains(m_pelletSprites[i].getPosition())) {
            m_pelletActive[i] = false;
        }
    }

    if (m_pacmanSprite.getPosition().x > m_window.getSize().x + 50) {
        m_pacmanSprite.setPosition({ -100.f, 185.f });
        for (size_t i = 0; i < m_pelletActive.size(); ++i) m_pelletActive[i] = true;
    }


    auto applyHover = [&](sf::Text& text, sf::Color hoverColor, bool isSoundText = false) {
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(hoverColor);
            text.setScale({ 1.1f, 1.1f });
        }
        else {
           
            if (isSoundText && !m_soundEnabled) {
                text.setFillColor(sf::Color::Red);
            }
            else {
                text.setFillColor(sf::Color::White);
            }
            text.setScale({ 1.0f, 1.0f });
        }
        };

    applyHover(m_soundToggle, sf::Color::Green, true);
    applyHover(m_backButton, sf::Color::Red, false);
}

void OptionsMenu::draw()
{
    for (const auto& star : m_stars) m_window.draw(star);
    for (size_t i = 0; i < m_pelletSprites.size(); ++i) {
        if (m_pelletActive[i]) m_window.draw(m_pelletSprites[i]);
    }
    m_window.draw(m_pacmanSprite);

    m_window.draw(m_title);
    m_window.draw(m_soundToggle);
    m_window.draw(m_backButton);
}
