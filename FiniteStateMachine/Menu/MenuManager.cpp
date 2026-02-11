#include "MenuManager.h"
#include <iostream>

MenuManager::MenuManager(sf::RenderWindow& window)
    : m_window(window),
    m_mainMenu(window),
    m_optionsMenu(window),
    m_endScreen(window),
    m_pauseText(m_font) 
{
    if (!m_font.openFromFile("../Assets/PressStart2P.ttf")) {
        std::cerr << "Erreur : police non chargee dans MenuManager\n";
    }

    m_pauseText.setString("PAUSE");
    m_pauseText.setCharacterSize(50);
    m_pauseText.setFillColor(sf::Color::Yellow);

    sf::FloatRect bounds = m_pauseText.getLocalBounds();
    m_pauseText.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    m_pauseText.setPosition({ window.getSize().x / 2.0f, window.getSize().y / 2.0f });
}

bool MenuManager::handleEvent(const std::optional<sf::Event>& event, GameState& currentState, sf::RenderWindow& window) {
    bool needReset = false;

    switch (currentState) {
    case GameState::Menu: {
        MainMenu::MenuAction action = m_mainMenu.handleEvent(*event);
        if (action == MainMenu::MenuAction::Play) {
            needReset = true;
            currentState = GameState::Game;
        }
        else if (action == MainMenu::MenuAction::Options) {
            currentState = GameState::Options;
        }
        break;
    }
    case GameState::Options: {
        int action = m_optionsMenu.handleEvent(*event);
        if (action == 1) currentState = GameState::Menu;
        break;
    }
    case GameState::End: {
        EndScreen::EndAction action = m_endScreen.handleEvent(*event);
        if (action == EndScreen::EndAction::Retry) {
            needReset = true;
            currentState = GameState::Game;
        }
        else if (action == EndScreen::EndAction::Menu) {
            currentState = GameState::Menu;
        }
        break;
    }
    default:
        break;
    }

    return needReset;
}

void MenuManager::update(GameState currentState) {
    switch (currentState) {
    case GameState::Menu:
        m_mainMenu.update();
        break;
    case GameState::Options:
        m_optionsMenu.update();
        break;
    case GameState::End:
        m_endScreen.update();
        break;
    default:
        break;
    }
}

void MenuManager::draw(GameState currentState) {
    switch (currentState) {
    case GameState::Menu:
        m_mainMenu.draw();
        break;
    case GameState::Options:
        m_optionsMenu.draw();
        break;
    case GameState::End:
        m_endScreen.draw();
        break;
    case GameState::Pause:
        m_window.draw(m_pauseText);
        break;
    default:
        break;
    }
}

void MenuManager::setupEndScreen(const std::string& message, sf::Color color) {
    m_endScreen.setup(message, color);
}