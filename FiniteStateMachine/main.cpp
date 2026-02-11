#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath> 
#include <algorithm>
#include <memory> // Nécessaire pour std::unique_ptr

// Fichiers du jeu
#include "Player.h"
#include "Background.h"
#include "Collectibles.h"
#include "StateMachine/NPC.h"
#include "NpcStates/NpcContext.h" 

// Fichiers des menus
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "EndScreen.h"

using namespace GhostAi;

// États globaux de l'application
enum class GameState {
    Menu,
    Options,
    Game,
    End
};

int main()
{
    // Initialisation de la graine aléatoire
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "PacMan - FSM Arena");
    window.setFramerateLimit(60);

    // --- Instanciation des Menus ---
    MainMenu mainMenu(window);
    OptionsMenu optionsMenu(window);
    EndScreen endScreen(window);

    // --- Instanciation des éléments du Jeu ---
    Background background;
    Collectibles collectibles;

    // Utilisation d'un pointeur intelligent pour le joueur
    // Cela permet de le supprimer et de le recréer proprement sans bug de texture
    std::unique_ptr<Player> player = std::make_unique<Player>();

    Npc blinky, pinky, inky, clyde;
    std::vector<Npc*> ghosts = { &blinky, &pinky, &inky, &clyde };
    sf::Vector2i startPos = { 14, 9 };

    // --- Variables de gestion du temps de jeu ---
    sf::Clock clock;
    float globalTimer = 0.f;
    GlobalGhostMode currentGlobalMode = GlobalGhostMode::Chase;
    float frightenedTimer = 0.f;
    bool isFrightenedMode = false;

    // État initial
    GameState currentState = GameState::Menu;

    // --- Fonction lambda pour réinitialiser le jeu (Appelée sur Play ou Retry) ---
    auto resetGame = [&]() {
        // On recrée le joueur proprement (nouvelle texture, nouvelle position)
        player = std::make_unique<Player>();

        collectibles.reset(); // Réactive toutes les gommes

        // Réinitialise les timers
        globalTimer = 0.f;
        isFrightenedMode = false;
        frightenedTimer = 0.f;
        currentGlobalMode = GlobalGhostMode::Chase;

        // Réinitialise les fantômes en leur donnant la nouvelle adresse du joueur
        // player.get() récupère le pointeur brut (Player*) requis par votre fonction Init
        blinky.Init(0, startPos, player.get());
        pinky.Init(1, startPos, player.get());
        inky.Init(3, startPos, player.get(), &blinky);
        clyde.Init(2, startPos, player.get());
        };

    // Premier setup au lancement
    resetGame();

    // --- Boucle Principale ---
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // 1. GESTION DES ÉVÉNEMENTS
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Gestion des clics selon l'état actuel
            switch (currentState) {
            case GameState::Menu: {
                MainMenu::MenuAction action = mainMenu.handleEvent(*event);
                if (action == MainMenu::MenuAction::Play) {
                    resetGame(); // On lance une nouvelle partie propre
                    currentState = GameState::Game;
                }
                else if (action == MainMenu::MenuAction::Options) {
                    currentState = GameState::Options;
                }
                break;
            }
            case GameState::Options: {
                int action = optionsMenu.handleEvent(*event);
                if (action == 1) { // Bouton Retour
                    currentState = GameState::Menu;
                }
                break;
            }
            case GameState::End: {
                EndScreen::EndAction action = endScreen.handleEvent(*event);
                if (action == EndScreen::EndAction::Retry) {
                    resetGame();
                    currentState = GameState::Game;
                }
                else if (action == EndScreen::EndAction::Menu) {
                    currentState = GameState::Menu;
                }
                break;
            }
            case GameState::Game:
                // Les inputs du joueur sont gérés dans player->update()
                break;
            }
        }

        // 2. MISE À JOUR (UPDATE)
        switch (currentState) {
        case GameState::Menu:
            mainMenu.update();
            break;

        case GameState::Options:
            optionsMenu.update();
            break;

        case GameState::End:
            endScreen.update();
            break;

        case GameState::Game:
            // --- Logique du jeu ---
            globalTimer += deltaTime;

            // Gestion des modes (Chase / Scatter / Frightened)
            if (isFrightenedMode) {
                frightenedTimer -= deltaTime;
                if (frightenedTimer <= 0.f) {
                    isFrightenedMode = false;
                    currentGlobalMode = GlobalGhostMode::Chase;
                }
                else {
                    currentGlobalMode = GlobalGhostMode::Frightened;
                }
            }
            else {
                if (globalTimer < 18.0f) currentGlobalMode = GlobalGhostMode::Chase;
                else if (globalTimer < 23.0f) currentGlobalMode = GlobalGhostMode::Scatter;
                else {
                    float loopTime = std::fmod(globalTimer - 23.0f, 20.0f);
                    if (loopTime < 15.0f) currentGlobalMode = GlobalGhostMode::Chase;
                    else currentGlobalMode = GlobalGhostMode::Scatter;
                }
            }

            // Mise à jour Joueur (avec la flèche -> car c'est un pointeur)
            player->update(deltaTime, background);

            // Gestion Hitbox Joueur
            sf::FloatRect playerHitbox = player->getGlobalBounds();
            playerHitbox.size.x *= 0.5f;
            playerHitbox.size.y *= 0.5f;
            playerHitbox.position.x += playerHitbox.size.x * 0.5f;
            playerHitbox.position.y += playerHitbox.size.y * 0.5f;

            // Collision avec les collectibles
            if (collectibles.checkCollision(playerHitbox)) {
                isFrightenedMode = true;
                frightenedTimer = 7.0f;
                currentGlobalMode = GlobalGhostMode::Frightened;
                std::cout << "GOMME MANGEE : MODE EFFRAYE ACTIVE (7s)" << std::endl;
            }

            // CONDITION DE VICTOIRE
            bool allEaten = std::all_of(collectibles.pellets.begin(), collectibles.pellets.end(),
                [](const Collectibles::PowerPellet& p) { return !p.isActive; });

            if (allEaten) {
                currentState = GameState::End;
                endScreen.setup("VICTOIRE", sf::Color::Green);
            }

            for (auto* ghost : ghosts) {
                ghost->Update(deltaTime, currentGlobalMode, background);

                sf::Vector2f gPos = ghost->GetWorldPosition();
                sf::FloatRect ghostHitbox({ gPos.x - 10.f, gPos.y - 10.f }, { 20.f, 20.f });

                if (playerHitbox.findIntersection(ghostHitbox)) {
                    if (isFrightenedMode) {
                        ghost->Respawn();
                    }
                    else {
                        currentState = GameState::End;
                        endScreen.setup("GAME OVER", sf::Color::Red);
                    }
                }
            }
            break;
        }

        window.clear(sf::Color::Black);

        switch (currentState) {
        case GameState::Menu:
            mainMenu.draw();
            break;

        case GameState::Options:
            optionsMenu.draw();
            break;

        case GameState::Game:
            background.draw(window);
            collectibles.draw(window);
            player->draw(window); 
            for (auto* ghost : ghosts) {
                ghost->Draw(window);
            }
            break;

        case GameState::End:
            endScreen.draw();
            break;
        }

        window.display();
    }

    return 0;
}