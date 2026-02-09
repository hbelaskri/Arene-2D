// Main.cpp
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath> // Pour std::fmod

#include "Player.h"
#include "Background.h"
#include "Collectibles.h"
#include "StateMachine/NPC.h"
#include "NpcStates/NpcContext.h" 

using namespace GhostAi;

int main()
{
    // Initialisation Aléatoire
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "PacMan - FSM Arena");
    window.setFramerateLimit(60);

    Background background;
    Player player;
    Collectibles collectibles;

    // --- Création des Fantômes ---
    Npc blinky, pinky, inky, clyde;

    // Positions de départ (Grid) - Centre de la carte
    sf::Vector2i startPos = { 14, 9 };

    // Initialisation avec dépendances
    blinky.Init(0, startPos, &player);
    pinky.Init(1, startPos, &player);
    inky.Init(3, startPos, &player, &blinky); // Inky a besoin de Blinky
    clyde.Init(2, startPos, &player);

    std::vector<Npc*> ghosts = { &blinky, &pinky, &inky, &clyde };

    sf::Clock clock;
    float globalTimer = 0.f;
    GlobalGhostMode currentGlobalMode = GlobalGhostMode::Chase;

    // --- Gestion des Vagues ---
    float frightenedTimer = 0.f;
    bool isFrightenedMode = false;

    while (window.isOpen())
    {
        // Gestion Événements SFML 3.0
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
        globalTimer += deltaTime;

        // --- Logique des Vagues (Conforme au prompt) ---
        if (isFrightenedMode) {
            frightenedTimer -= deltaTime;
            if (frightenedTimer <= 0.f) {
                isFrightenedMode = false;
                // On reprend le timer global là où il en était approximativement 
                // ou on force le mode Chase pour la sécurité
                currentGlobalMode = GlobalGhostMode::Chase;
            }
            else {
                currentGlobalMode = GlobalGhostMode::Frightened;
            }
        }
        else {
            // Séquence : Traque (jusqu'à 18s pour couvrir le start) -> Dispersion (5s) -> Boucle (15s Traque / 5s Dispersion)
            if (globalTimer < 18.0f) {
                currentGlobalMode = GlobalGhostMode::Chase;
            }
            else if (globalTimer < 23.0f) {
                currentGlobalMode = GlobalGhostMode::Scatter;
            }
            else {
                // Boucle de 20 secondes (15s Chase + 5s Scatter)
                float loopTime = std::fmod(globalTimer - 23.0f, 20.0f);
                if (loopTime < 15.0f) {
                    currentGlobalMode = GlobalGhostMode::Chase;
                }
                else {
                    currentGlobalMode = GlobalGhostMode::Scatter;
                }
            }
        }

        // --- Mises à jour ---
        player.update(deltaTime, background);

        // --- GESTION HITBOX JOUEUR (SFML 3.0) ---
        sf::FloatRect playerHitbox = player.getGlobalBounds();

        // Réduction de la hitbox pour être plus permissif ("gentil" avec le joueur)
        playerHitbox.size.x *= 0.5f;
        playerHitbox.size.y *= 0.5f;
        // Recentrage après réduction
        playerHitbox.position.x += playerHitbox.size.x * 0.5f;
        playerHitbox.position.y += playerHitbox.size.y * 0.5f;

        // --- Collision avec les Gommes (Mode Effrayé) ---
        if (collectibles.checkCollision(playerHitbox)) {
            isFrightenedMode = true;
            frightenedTimer = 7.0f; // Durée fixée par le CDC
            currentGlobalMode = GlobalGhostMode::Frightened;
            std::cout << "GOMME MANGEE : MODE EFFRAYE ACTIVE (7s)" << std::endl;
        }

        // --- Mise à jour et Collision des Fantômes ---
        for (auto* ghost : ghosts) {
            ghost->Update(deltaTime, currentGlobalMode, background);

            // Création Hitbox Fantôme (SFML 3.0 nécessite {pos}, {size})
            sf::Vector2f gPos = ghost->GetWorldPosition();
            // Hitbox un peu plus petite que la tuile (20x20 au lieu de 25x25)
            sf::FloatRect ghostHitbox({ gPos.x - 10.f, gPos.y - 10.f }, { 20.f, 20.f });

            if (playerHitbox.findIntersection(ghostHitbox)) {
                if (isFrightenedMode) {
                    // --- CORRECTION MAJEURE ---
                    // Au lieu de Init() qui recrée la FSM et cause des fuites/bugs,
                    // on utilise Respawn() pour simplement remettre le fantôme au départ.
                    ghost->Respawn();
                    std::cout << "FANTOME MANGE !" << std::endl;
                }
                else {
                    std::cout << "GAME OVER - Touche par un fantome !" << std::endl;

                    // Reset complet du jeu
                    player = Player(); // Reset joueur (constructeur par défaut)
                    globalTimer = 0.f;
                    isFrightenedMode = false;
                    currentGlobalMode = GlobalGhostMode::Chase;

                    // Réinitialisation des collectibles
                    collectibles.reset();

                    // Réinitialisation propre des fantômes
                    // Ici on utilise Init car c'est un reset total du jeu
                    blinky.Init(0, startPos, &player);
                    pinky.Init(1, startPos, &player);
                    inky.Init(3, startPos, &player, &blinky);
                    clyde.Init(2, startPos, &player);
                }
            }
        }

        // --- Rendu ---
        window.clear(sf::Color::Black);

        background.draw(window);
        collectibles.draw(window);
        player.draw(window);

        for (auto* ghost : ghosts) {
            ghost->Draw(window);
        }

        window.display();
    }

    return 0;
}