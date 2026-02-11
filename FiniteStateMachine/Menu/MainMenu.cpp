#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow& window)
    : m_window(window),
    m_title(m_font, ""), m_play(m_font, ""), m_quit(m_font, ""), m_options(m_font, "")
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf"))
    {
        std::cerr << "Erreur : police non chargee\n";
    }

    float centerX = m_window.getSize().x / 2.0f;

    m_title.setString("PACMAN");
    m_title.setCharacterSize(64);
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect tB = m_title.getLocalBounds();
    m_title.setOrigin({ tB.size.x / 2.0f, tB.size.y / 2.0f });
    m_title.setPosition({ centerX, 80.f });

    m_play.setString("PLAY");
    m_play.setCharacterSize(36);
    sf::FloatRect pB = m_play.getLocalBounds();
    m_play.setOrigin({ pB.size.x / 2.0f, pB.size.y / 2.0f });
    m_play.setPosition({ centerX, 250.f });

    m_quit.setString("QUIT");
    m_quit.setCharacterSize(36);
    sf::FloatRect qB = m_quit.getLocalBounds();
    m_quit.setOrigin({ qB.size.x / 2.0f, qB.size.y / 2.0f });
    m_quit.setPosition({ centerX, 320.f });

    m_options.setString("OPTIONS");
    m_options.setCharacterSize(36);
    sf::FloatRect oB = m_options.getLocalBounds();
    m_options.setOrigin({ oB.size.x / 2.0f, oB.size.y / 2.0f });
    m_options.setPosition({ centerX, 390.f });
}

MainMenu::MenuAction MainMenu::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::Closed>()) m_window.close();

    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseClick->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(mouseClick->position.x),
                static_cast<float>(mouseClick->position.y));

            if (m_quit.getGlobalBounds().contains(mousePos)) m_window.close();

         
            if (m_play.getGlobalBounds().contains(mousePos)) return MenuAction::Play;
            if (m_options.getGlobalBounds().contains(mousePos)) return MenuAction::Options;
        }
    }
    return MenuAction::None;
}

void MainMenu::update()
{
    sf::Vector2i mouseI = sf::Mouse::getPosition(m_window);
    sf::Vector2f mousePos(static_cast<float>(mouseI.x), static_cast<float>(mouseI.y));

    if (m_play.getGlobalBounds().contains(mousePos)) {
        m_play.setFillColor(sf::Color::Blue);
        m_play.setScale({ 1.1f, 1.1f });
    }
    else {
        m_play.setFillColor(sf::Color::White);
        m_play.setScale({ 1.0f, 1.0f });
    }

    if (m_quit.getGlobalBounds().contains(mousePos)) {
        m_quit.setFillColor(sf::Color::Red);
        m_quit.setScale({ 1.1f, 1.1f });
    }
    else {
        m_quit.setFillColor(sf::Color::White);
        m_quit.setScale({ 1.0f, 1.0f });
    }

    if (m_options.getGlobalBounds().contains(mousePos)) {
        m_options.setFillColor(sf::Color::Green);
        m_options.setScale({ 1.1f, 1.1f });
    }
    else {
        m_options.setFillColor(sf::Color::White);
        m_options.setScale({ 1.0f, 1.0f });
    }
}

void MainMenu::draw()
{
    m_window.draw(m_title);
    m_window.draw(m_play);
    m_window.draw(m_quit);
    m_window.draw(m_options);
}