#pragma once

#include <SFML/Graphics.hpp>
#include "StateMachine/StateMachine.h"
#include "NpcStates/NpcContext.h"
#include "NpcStates/ChaseState.h" 
#include "Directions.h"

// Forward declaration 
class Player;
class Background;

using namespace GhostAi;

class Npc : public IGhostEntity
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    // Animation 
    int currentFrame;
    float frameTime;
    float animationSpeed;
    sf::Vector2i frameSize;
    sf::Color baseColor;

    float speed = 100.f;
    sf::Vector2i startGridPosition; // Sauvegarde pour le respawn

    // Références pour l'IA
    Player* targetPlayer = nullptr;
    Npc* blinkyReference = nullptr;

public:
    FSM::StateMachine<GhostContext> fsm;
    GhostContext context{};

    Npc();

    void Init(int id, sf::Vector2i startGridPos, Player* player, Npc* blinky = nullptr);
    void Respawn(); // Nouvelle méthode pour gérer la mort sans fuite mémoire

    void Update(float dt, GlobalGhostMode currentMode, const Background& bg);
    void Draw(sf::RenderWindow& window);

    // --- Implémentation IGhostEntity ---
    void SetTargetToPlayer() override;
    void SetTargetToCorner() override;
    void SetTargetToRandom() override;
    void SetSpeed(float speed) override;
    void ReverseDirection() override;

    sf::Vector2i GetGridPosition() const;
    sf::Vector2f GetWorldPosition() const { return sprite.getPosition(); }

};