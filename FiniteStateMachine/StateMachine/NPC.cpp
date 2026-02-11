#include "NPC.h"
#include "Player.h"     
#include "Background.h"
#include "Navigation.h"
#include "NpcStates/Conditions.h"
#include "NpcStates/ChaseState.h" 
#include <iostream>
#include <cmath>

Npc::Npc()
    : sprite(texture), currentFrame(0), frameTime(0.f), animationSpeed(0.1f)
{
}

void Npc::Init(int id, sf::Vector2i startGridPos, Player* player, Npc* blinky)
{
    this->targetPlayer = player;
    this->blinkyReference = blinky;

    context.ghost = this;
    context.ghostID = id; 

    SetTargetToCorner();
    sf::Vector2i spawnPos = context.targetGridPos;

    this->startGridPosition = spawnPos;

    context.currentGlobalMode = GlobalGhostMode::Chase;
    context.currentDirection = Direction::None;

    std::string filename;
    switch (id) {
    case 0: filename = "redGhost.png"; break;
    case 1: filename = "yellowGhost.png"; break;
    case 2: filename = "greenGhost.png"; break;
    case 3: filename = "blueGhost.png"; break;
    default: filename = "redGhost.png"; break;
    }

    if (!texture.loadFromFile("../Assets/pacmanPack/" + filename)) {
        std::cerr << "Erreur texture: " << filename << std::endl;
    }

    sf::Vector2u texSize = texture.getSize();
    frameSize.x = static_cast<int>(texSize.x) / 8;
    frameSize.y = static_cast<int>(texSize.y);

    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize.x, frameSize.y }));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    float targetSize = Background::TILE_SIZE * 0.8f;
    float scale = targetSize / frameSize.x;
    sprite.setScale({ scale, scale });

    sprite.setPosition(Navigation::GridToWorld(spawnPos));

    baseColor = sf::Color::White;
    sprite.setColor(baseColor);

    auto chaseState = fsm.CreateState<GhostAi::ChaseState>();
    auto scatterState = fsm.CreateState<GhostAi::ScatterState>();
    auto fearState = fsm.CreateState<GhostAi::FrightenedState>();

    chaseState->AddTransition(GhostAi::Conditions::ShouldScatter, scatterState);
    chaseState->AddTransition(GhostAi::Conditions::IsFrightened, fearState);

    scatterState->AddTransition(GhostAi::Conditions::ShouldChase, chaseState);
    scatterState->AddTransition(GhostAi::Conditions::IsFrightened, fearState);

    fearState->AddTransition(GhostAi::Conditions::IsNotFrightened, chaseState);

    fsm.Init(chaseState, context);
}

void Npc::Respawn() {
    SetTargetToCorner();

    sf::Vector2i cornerPos = context.targetGridPos;

    sprite.setPosition(Navigation::GridToWorld(cornerPos));

    context.currentDirection = Direction::None;
    context.timeInCurrentState = 0.f;

    speed = 100.f;
    sprite.setColor(baseColor);

    std::cout << "Ghost " << context.ghostID << " respawned at corner." << std::endl;
}

void Npc::Update(float dt, GlobalGhostMode currentMode, const Background& bg)
{
    context.currentGlobalMode = currentMode;
    context.timeInCurrentState += dt;

    fsm.Update(context);

    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2i currentGridPos = Navigation::WorldToGrid(currentPos);
    sf::Vector2f centerOfTile = Navigation::GridToWorld(currentGridPos);

    float distToCenter = std::sqrt(std::pow(currentPos.x - centerOfTile.x, 2) + std::pow(currentPos.y - centerOfTile.y, 2));

    if (distToCenter < 1.0f)
    {
        sprite.setPosition(centerOfTile); // Snap to grid

        context.currentDirection = Navigation::GetNextGhostDirection(
            currentGridPos,
            context.currentDirection,
            context.targetGridPos,
            bg
        );
    }

    sf::Vector2i dirVec = DirToVec(context.currentDirection);
    sf::Vector2f movement = sf::Vector2f(static_cast<float>(dirVec.x), static_cast<float>(dirVec.y)) * speed * dt;

    sf::Vector2f nextPos = sprite.getPosition() + movement;
    if (Navigation::IsWalkable(Navigation::WorldToGrid(nextPos).x, Navigation::WorldToGrid(nextPos).y, bg)) {
        sprite.move(movement);
    }

    if (currentMode == GlobalGhostMode::Frightened) {
        float remainingTime = 7.0f - context.timeInCurrentState;
        sf::Color fearColor(148, 0, 211);

        if (remainingTime <= 2.0f && remainingTime > 0.0f) {
            int flash = static_cast<int>(remainingTime * 5.0f);
            if (flash % 2 == 0) {
                sprite.setColor(baseColor);
            }
            else {
                sprite.setColor(fearColor);
            }
        }
        else {
            sprite.setColor(fearColor);
        }
    }
    else {
        sprite.setColor(baseColor);
    }

    frameTime += dt;
    if (frameTime >= animationSpeed)
    {
        frameTime = 0.f;
        currentFrame = (currentFrame + 1) % 8;
        sprite.setTextureRect(sf::IntRect({ currentFrame * frameSize.x, 0 }, { frameSize.x, frameSize.y }));
    }
}

void Npc::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Npc::SetTargetToPlayer() {
    if (!targetPlayer) return;

    sf::Vector2i playerPos = targetPlayer->getGridPosition();
    Direction playerDir = targetPlayer->getDirection();
    sf::Vector2i playerDirVec = DirToVec(playerDir);

    if (context.ghostID == 0) {
        context.targetGridPos = playerPos;
    }
    else if (context.ghostID == 1) {
        context.targetGridPos = playerPos + (playerDirVec * 4);
    }
    else if (context.ghostID == 2) {
        float distSq = Navigation::GetDistanceSq(GetGridPosition(), playerPos);
        if (distSq > 64.0f) {
            context.targetGridPos = playerPos;
        }
        else {
            context.targetGridPos = { 1, 18 };
        }
    }
    else if (context.ghostID == 3) {
        if (blinkyReference) {
            sf::Vector2i pivot = playerPos + (playerDirVec * 2);
            sf::Vector2i blinkyPos = blinkyReference->GetGridPosition();
            sf::Vector2i vectorBlinkyToPivot = pivot - blinkyPos;
            context.targetGridPos = pivot + vectorBlinkyToPivot;
        }
        else {
            context.targetGridPos = playerPos;
        }
    }
}

void Npc::SetTargetToCorner() {
    switch (context.ghostID) {
    case 0: context.targetGridPos = { 28, 1 }; break;  // Haut Droite
    case 1: context.targetGridPos = { 1, 1 }; break;   // Haut Gauche
    case 2: context.targetGridPos = { 1, 18 }; break;  // Bas Gauche
    case 3: context.targetGridPos = { 28, 18 }; break; // Bas Droite
    default: context.targetGridPos = { 1, 1 }; break;
    }
}

void Npc::SetTargetToRandom() {
    int randX = rand() % Background::MAP_WIDTH;
    int randY = rand() % Background::MAP_HEIGHT;
    context.targetGridPos = { randX, randY };
}

void Npc::SetSpeed(float newSpeed) {
    speed = newSpeed;
}

void Npc::ReverseDirection() {
    context.currentDirection = GetOpposite(context.currentDirection);
}

sf::Vector2i Npc::GetGridPosition() const {
    return Navigation::WorldToGrid(sprite.getPosition());
}
