# Mini-arène 2D – FSM et IA

## Contexte

Dans ce projet, nous développons une **mini-arène 2D** en **C++ avec SFML**, dans laquelle évoluent :
- **4 PNJ (agents)** pilotés par une **Intelligence Artificielle (FSM générique)**
- **1 joueur** contrôlé au clavier

Chaque PNJ est géré par une **Finite State Machine générique (template)**, réutilisable pour différents types d’agents. 
Le projet met l’accent sur la **séparation entre la logique de décision (FSM, états, transitions) et le reste du système** (affichage, entrées, moteur de jeu).

--- 
## Objectif principal

L’objectif principal est de **vérifier notre compréhension et notre mise en œuvre concrète des Finite State Machines (FSM)** et de la **programmation générique par templates** dans un contexte applicatif.

Nous devons :
- Concevoir une **IA modulaire et réutilisable** pour les PNJ
- Séparer clairement la **logique décisionnelle** du **moteur de jeu et de l’affichage**

---

## Contrôles du Player

| Touche | Action | 
|--------|--------|
| Z | Déplacement vers le haut| 
| S | Déplacement vers le bas | 
| Q | Déplacement vers la gauche | 
| D | Déplacement vers la droite | 
---

## Contrôles des menus 
-Les interactions avec les menus se font a la souris.

## Structure du projet

```
Project/
│
├── Assets/                     # Ressources (sprites, images)
│
├── Menu/                       # Gestion des menus
│   ├── MainMenu.h /.cpp
│   ├── OptionsMenu.h /.cpp
│   ├── EndScreen.h /.cpp
│   └── MenuManager.h /.cpp
│
├── NpcStates/                  # États spécifiques des PNJ
│   ├── ChaseState.h
│   ├── PatrolState.h
│   ├── Conditions.h
│   └── NpcContext.h
│
├── StateMachine/               # Implémentation générique de la FSM
│
├── Background.h /.cpp
├── Collectibles.h /.cpp
├── Navigation.h /.cpp
├── Player.h /.cpp
├── Game.h /.cpp
│
└── main.cpp                    # Point d’entrée du programme
```

## Choix technologiques

Pour ce projet, nous avons utilisé les technologies suivantes :
- **C++ (norme C++17 minimum)** Nous utilisons C++17 afin de pouvoir tirer parti de la **programmation générique par templates** et d’encourager une conception **orientée performance et architecture**.

- **SFML (Simple and Fast Multimedia Library)** SFML est utilisée pour :
- La gestion de la **fenêtre**
- L’**affichage 2D**
- La lecture des **entrées clavier**
- La **temporisation** (delta time)
- la **Map**

## Map

- Map éditée à la main avec des **blocs SFML**
- Sauvegardée dans un fichier `map.txt` pour être réutilisée par le moteur
- La map définit les murs, chemins et zones pour les PNJ et le joueur.


##  Améliorations envisagées

Plusieurs améliorations auraient pu être ajoutées :
 
- Création de plusieurs niveaux avec difficulté progressive.
- Ajout d’un système de score.
- Ajout de sons et musiques.

---

### Compatibilité

 Le projet doit pouvoir être compilé sur les **environnements standards du cursus (Windows)**.

  ## Crédits

  -Hugo Leroyer
  
  -Hania Belaskri

  




















