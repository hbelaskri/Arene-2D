#include "EndScreen.h"

EndScreen::EndScreen(sf::RenderWindow& window)
    : m_window(window),
    m_messageText(m_font, ""),
    m_retryButton(m_font, ""),
    m_menuButton(m_font, "")
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf")) {
        std::cerr << "Erreur : police non chargee dans EndScreen\n";
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
        float time = m_animationClock.getElapsedTime().asSeconds();

        int state = static_cast<int>(time * 10) % 3;

        if (state == 0) m_messageText.setFillColor(sf::Color::Yellow);
        else if (state == 1) m_messageText.setFillColor(sf::Color::Green);
        else m_messageText.setFillColor(sf::Color::White);
    }
}

void EndScreen::draw()
{
    m_window.draw(m_messageText);
    m_window.draw(m_retryButton);
    m_window.draw(m_menuButton);
}