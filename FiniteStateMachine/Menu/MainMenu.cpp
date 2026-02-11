#include "MainMenu.h"
#include <cmath>

MainMenu::MainMenu(sf::RenderWindow& window)
    : m_window(window),
    m_title(m_font, ""), m_play(m_font, ""), m_quit(m_font, ""), m_options(m_font, ""),
    m_pacmanSprite(m_pacmanTexture)
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf")) {
        std::cerr << "Erreur : police non chargee\n";
    }

  
    if (!m_pacmanTexture.loadFromFile("../Assets/pacmanPack/PacMan.png")) {
        std::cerr << "Erreur : PacMan.png non trouve\n";
    }
    if (!m_coinTexture.loadFromFile("../Assets/pacmanPack/Coin.png")) {
        std::cerr << "Erreur : Coin.png non trouve\n";
    }

    m_pacmanFrameSize.x = static_cast<int>(m_pacmanTexture.getSize().x) / 8;
    m_pacmanFrameSize.y = static_cast<int>(m_pacmanTexture.getSize().y);

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
        s.setPosition({ i * 80.f, 165.f });

        m_pelletSprites.push_back(s);
        m_pelletActive.push_back(true);
    }

    m_pacmanSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), m_pacmanFrameSize));
    m_pacmanSprite.setOrigin({ static_cast<float>(m_pacmanFrameSize.x) / 2.0f, static_cast<float>(m_pacmanFrameSize.y) / 2.0f });
    m_pacmanSprite.setScale({ 2.0f, 2.0f });
    m_pacmanSprite.setPosition({ -50.f, 165.f });
}

MainMenu::MenuAction MainMenu::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) m_window.close();

    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(static_cast<float>(mouseClick->position.x),
                static_cast<float>(mouseClick->position.y));
            if (m_quit.getGlobalBounds().contains(mousePos)) m_window.close();
            if (m_play.getGlobalBounds().contains(mousePos)) return MenuAction::Play;
            if (m_options.getGlobalBounds().contains(mousePos)) return MenuAction::Options;
        }
    }
    return MenuAction::None;
}

void MainMenu::update() {
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
        m_pacmanSprite.setPosition({ -100.f, 165.f });
        for (size_t i = 0; i < m_pelletActive.size(); ++i) m_pelletActive[i] = true;
    }

    auto applyHover = [&](sf::Text& text, sf::Color hoverColor) {
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(hoverColor);
            text.setScale({ 1.15f, 1.15f });
        }
        else {
            text.setFillColor(sf::Color::White);
            text.setScale({ 1.0f, 1.0f });
        }
        };

    applyHover(m_play, sf::Color::Blue);
    applyHover(m_quit, sf::Color::Red);
    applyHover(m_options, sf::Color::Green);
}

void MainMenu::draw() {
    for (const auto& star : m_stars) m_window.draw(star);

    for (size_t i = 0; i < m_pelletSprites.size(); ++i) {
        if (m_pelletActive[i]) m_window.draw(m_pelletSprites[i]);
    }

    m_window.draw(m_pacmanSprite);
    m_window.draw(m_title);
    m_window.draw(m_play);
    m_window.draw(m_quit);
    m_window.draw(m_options);
}