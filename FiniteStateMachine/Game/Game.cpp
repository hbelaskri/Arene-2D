#include "Game.h"
#include <ctime>
#include <cmath>
#include <iostream>

using namespace GhostAi;

Game::Game()
    : m_window(sf::VideoMode({ 800, 600 }), "PacMan - FSM Arena"),
    m_menuManager(m_window),
    m_currentState(GameState::Menu),
    m_background(),
    m_collectibles(m_background),
    m_startPos(14, 9),
    m_globalTimer(0.f),
    m_frightenedTimer(0.f),
    m_isFrightenedMode(false),
    m_currentGlobalMode(GlobalGhostMode::Chase)
{
    m_window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    m_ghosts = { &blinky, &pinky, &inky, &clyde };

    resetGame();
}

void Game::run() {
    while (m_window.isOpen()) {
        processEvents();

        float deltaTime = m_clock.restart().asSeconds();
        update(deltaTime);

        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        if (m_menuManager.handleEvent(event, m_currentState, m_window)) {
            resetGame();
        }
    }
}

void Game::resetGame() {
    m_player = std::make_unique<Player>();
    m_collectibles.reset();

    m_globalTimer = 0.f;
    m_isFrightenedMode = false;
    m_frightenedTimer = 0.f;
    m_currentGlobalMode = GlobalGhostMode::Chase;

    blinky.Init(0, m_startPos, m_player.get());
    pinky.Init(1, m_startPos, m_player.get());
    inky.Init(3, m_startPos, m_player.get(), &blinky);
    clyde.Init(2, m_startPos, m_player.get());
}

void Game::update(float deltaTime) {
    if (m_currentState != GameState::Game) {
        m_menuManager.update(m_currentState);
        return;
    }

    m_globalTimer += deltaTime;

    if (m_isFrightenedMode) {
        m_frightenedTimer -= deltaTime;
        if (m_frightenedTimer <= 0.f) {
            m_isFrightenedMode = false;
            m_currentGlobalMode = GlobalGhostMode::Chase;
        }
        else {
            m_currentGlobalMode = GlobalGhostMode::Frightened;
        }
    }
    else {
        if (m_globalTimer < 18.0f) m_currentGlobalMode = GlobalGhostMode::Chase;
        else if (m_globalTimer < 23.0f) m_currentGlobalMode = GlobalGhostMode::Scatter;
        else {
            float loopTime = std::fmod(m_globalTimer - 23.0f, 20.0f);
            if (loopTime < 15.0f) m_currentGlobalMode = GlobalGhostMode::Chase;
            else m_currentGlobalMode = GlobalGhostMode::Scatter;
        }
    }

    m_player->update(deltaTime, m_background);
    m_collectibles.update(deltaTime);

    sf::FloatRect playerHitbox = m_player->getGlobalBounds();
    playerHitbox.size.x *= 0.5f;
    playerHitbox.size.y *= 0.5f;
    playerHitbox.position.x += playerHitbox.size.x * 0.5f;
    playerHitbox.position.y += playerHitbox.size.y * 0.5f;

    if (m_collectibles.checkCollision(playerHitbox)) {
        m_isFrightenedMode = true;
        m_frightenedTimer = 7.0f;
        m_currentGlobalMode = GlobalGhostMode::Frightened;
        std::cout << "BIG COIN MANGEE : MODE EFFRAYE ACTIVE (7s)" << std::endl;
    }

    if (m_collectibles.allCollected()) {
        m_currentState = GameState::End;
        m_menuManager.setupEndScreen("VICTOIRE", sf::Color::Green);
    }

    for (auto* ghost : m_ghosts) {
        ghost->Update(deltaTime, m_currentGlobalMode, m_background);

        sf::Vector2f gPos = ghost->GetWorldPosition();
        sf::FloatRect ghostHitbox({ gPos.x - 10.f, gPos.y - 10.f }, { 20.f, 20.f });

        if (playerHitbox.findIntersection(ghostHitbox)) {
            if (m_isFrightenedMode) {
                ghost->Respawn();
            }
            else {
                m_currentState = GameState::End;
                m_menuManager.setupEndScreen("GAME OVER", sf::Color::Red);
            }
        }
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black);

    if (m_currentState == GameState::Game) {
        m_background.draw(m_window);
        m_collectibles.draw(m_window);
        m_player->draw(m_window);
        for (auto* ghost : m_ghosts) {
            ghost->Draw(m_window);
        }
    }
    else {
        m_menuManager.draw(m_currentState);
    }

    m_window.display();
}