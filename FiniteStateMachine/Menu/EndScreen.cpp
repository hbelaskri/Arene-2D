#include "EndScreen.h"
#include <cmath>

EndScreen::EndScreen(sf::RenderWindow& window)
    : m_window(window),
    m_messageText(m_font, ""),
    m_retryButton(m_font, ""),
    m_menuButton(m_font, "")
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf")) {
        std::cerr << "Erreur : police non chargee dans EndScreen\n";
    }

   
    for (int i = 0; i < 60; ++i) {
        sf::CircleShape star(1.5f);
        star.setPosition({ static_cast<float>(rand() % m_window.getSize().x),
                           static_cast<float>(rand() % m_window.getSize().y) });
        star.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(rand() % 180)));
        m_stars.push_back(star);
    }

 
    if (m_ghostTexture.loadFromFile("../Assets/pacmanPack/redGhost.png")) {
        for (int i = 0; i < 4; i++) {
            sf::Sprite s(m_ghostTexture);
            int frameW = static_cast<int>(m_ghostTexture.getSize().x) / 8;
            int frameH = static_cast<int>(m_ghostTexture.getSize().y);
            s.setTextureRect(sf::IntRect({ 0, 0 }, { frameW, frameH }));
            s.setScale({ 2.5f, 2.5f });
            s.setOrigin({ frameW / 2.0f, frameH / 2.0f });

            float x = static_cast<float>(rand() % (m_window.getSize().x - 100) + 50);
            float y = static_cast<float>(rand() % (m_window.getSize().y - 100) + 50);
            s.setPosition({ x, y });

            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
            m_ghostDirections.push_back({ std::cos(angle), std::sin(angle) });

            if (i == 0) s.setColor(sf::Color::Red);
            if (i == 1) s.setColor(sf::Color::Cyan);
            if (i == 2) s.setColor(sf::Color(255, 182, 193));
            if (i == 3) s.setColor(sf::Color::Green);
            m_visualGhosts.push_back(s);
        }
    }

    float centerX = m_window.getSize().x / 2.0f;
    m_retryButton.setString("RETRY");
    m_retryButton.setCharacterSize(36);
    sf::FloatRect rb = m_retryButton.getLocalBounds();
    m_retryButton.setOrigin({ rb.size.x / 2.0f, rb.size.y / 2.0f });
    m_retryButton.setPosition({ centerX, 300.f });

    m_menuButton.setString("MENU");
    m_menuButton.setCharacterSize(36);
    sf::FloatRect mb = m_menuButton.getLocalBounds();
    m_menuButton.setOrigin({ mb.size.x / 2.0f, mb.size.y / 2.0f });
    m_menuButton.setPosition({ centerX, 380.f });
}

void EndScreen::setup(const std::string& message, sf::Color color)
{
    m_messageText.setString(message);
    m_messageText.setCharacterSize(60);
    m_messageText.setFillColor(color);
    m_isVictory = (message == "VICTOIRE");
    m_animationClock.restart();

    sf::FloatRect b = m_messageText.getLocalBounds();
    m_messageText.setOrigin({ b.size.x / 2.0f, b.size.y / 2.0f });
    m_messageText.setPosition({ m_window.getSize().x / 2.0f, 120.f });
}

EndScreen::EndAction EndScreen::handleEvent(const sf::Event& event)
{
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(static_cast<float>(mouseClick->position.x),
                static_cast<float>(mouseClick->position.y));
            if (m_retryButton.getGlobalBounds().contains(mousePos)) return EndAction::Retry;
            if (m_menuButton.getGlobalBounds().contains(mousePos)) return EndAction::Menu;
        }
    }
    return EndAction::None;
}

void EndScreen::update()
{
    sf::Vector2i mouseI = sf::Mouse::getPosition(m_window);
    sf::Vector2f mousePos(static_cast<float>(mouseI.x), static_cast<float>(mouseI.y));
    float time = m_animationClock.getElapsedTime().asSeconds();

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

    applyHover(m_retryButton, sf::Color::Yellow);
    applyHover(m_menuButton, sf::Color::Cyan);

    if (m_isVictory) {
      
        float r = (std::sin(time * 5.0f) * 127 + 128);
        float g = (std::sin(time * 5.0f + 2.09f) * 127 + 128);
        float b = (std::sin(time * 5.0f + 4.18f) * 127 + 128);
        m_messageText.setFillColor(sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)));
    }
    else {
       
        int alphaVal = static_cast<int>(180 + 75 * std::sin(time * 6.0f));
        sf::Color c = m_messageText.getFillColor();
        m_messageText.setFillColor(sf::Color(c.r, c.g, c.b, static_cast<uint8_t>(alphaVal)));

        float speed = 200.f;
        float dt = 1.0f / 60.0f;
        for (size_t i = 0; i < m_visualGhosts.size(); ++i) {
            m_visualGhosts[i].move(m_ghostDirections[i] * speed * dt);

            if (m_visualGhosts[i].getPosition().x <= 0 || m_visualGhosts[i].getPosition().x >= m_window.getSize().x)
                m_ghostDirections[i].x *= -1;
            if (m_visualGhosts[i].getPosition().y <= 0 || m_visualGhosts[i].getPosition().y >= m_window.getSize().y)
                m_ghostDirections[i].y *= -1;

            int frameW = static_cast<int>(m_ghostTexture.getSize().x) / 8;
            int frameIdx = static_cast<int>(time * 8) % 2;
            m_visualGhosts[i].setTextureRect(sf::IntRect({ frameIdx * frameW, 0 }, { frameW, static_cast<int>(m_ghostTexture.getSize().y) }));
        }
    }
}

void EndScreen::draw()
{
    for (const auto& star : m_stars) m_window.draw(star);

  
    if (!m_isVictory) {
        for (const auto& ghost : m_visualGhosts) m_window.draw(ghost);
    }

    m_window.draw(m_messageText);
    m_window.draw(m_retryButton);
    m_window.draw(m_menuButton);
}
