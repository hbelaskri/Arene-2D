#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Menu/MenuManager.h" 
#include "Player.h"
#include "Background.h"
#include "Collectibles.h"
#include "StateMachine/NPC.h"
#include "NpcStates/NpcContext.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void resetGame();

private:
    sf::RenderWindow m_window;
    MenuManager m_menuManager;
    GameState m_currentState;

    Background m_background;
    Collectibles m_collectibles;
    std::unique_ptr<Player> m_player;

    Npc blinky, pinky, inky, clyde;
    std::vector<Npc*> m_ghosts;
    sf::Vector2i m_startPos;

    sf::Clock m_clock;
    float m_globalTimer;
    float m_frightenedTimer;
    bool m_isFrightenedMode;

    GlobalGhostMode m_currentGlobalMode;
};