#include "OptionsMenu.h"

OptionsMenu::OptionsMenu(sf::RenderWindow& window)
    : m_window(window), m_title(m_font, ""), m_soundToggle(m_font, ""), m_backButton(m_font, "")
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf")) {
        std::cerr << "Erreur : police non chargee\n";
    }

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

    auto applyHover = [&](sf::Text& text, sf::Color hoverColor) {
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(hoverColor);
            text.setScale({ 1.1f, 1.1f });
        }
        else {
            text.setFillColor(sf::Color::White);
            text.setScale({ 1.0f, 1.0f });
        }
        };

    applyHover(m_soundToggle, sf::Color::Green);
    applyHover(m_backButton, sf::Color::Red);
}

void OptionsMenu::draw()
{
    m_window.draw(m_title);
    m_window.draw(m_soundToggle);
    m_window.draw(m_backButton);
}