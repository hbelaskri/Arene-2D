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
- Produire un code **lisible, bien modélisé et extensible**

---

## Thème choisi

**Thème libre : Pacman** 
Nous avons choisi **Pacman** comme thème car il illustre parfaitement l’utilisation de **FSM et d’IA** :
- Les fantômes sont des agents pilotés par une FSM avec différents états (Chase, Scatter, Frightened)
- Le joueur est contrôlé par le clavier et interagit avec la map et les agents.
- Le jeu nécessite de **séparer la logique de décision des personnages de l’animation et du rendu graphique**

Ce choix nous permet de mettre en pratique **les concepts théoriques de FSM et LIA**.

## Objectif du jeu

-Le joueur contrôle un personnage qui se déplace dans un labyrinthe.

Son But : 

- Collecter tous les objets présents sur la map.
- Éviter les ennemis (Fantomes).
- Survivre jusqu’à ce que tous les collectibles soient récupérés.

Si le joueur est attrapé par un ennemi → la partie se termine (Game Over).

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

Pour ce projet, nous avons utilisons les technologies suivantes :
- **C++ (norme C++17 minimum)** Nous utilisons C++17 afin de pouvoir tirer parti de la **programmation générique par templates** et d’encourager une conception **orientée performance et architecture**.

- **SFML (Simple and Fast Multimedia Library)** SFML est utilisée pour :
- La gestion de la **fenêtre**
- L’**affichage 2D** - La lecture des **entrées clavier**
- La **temporisation** (delta time)
- la **Map**

## Map

- Map éditée à la main avec des **blocs SFML**
- Sauvegardée dans un fichier `map.txt` pour être réutilisée par le moteur
- La map définit les murs, chemins et zones pour les PNJ et le joueur.

##  Comportements uniques des différents fantômes

Chaque fantôme possède un comportement spécifique implémenté via une FSM, ce qui rend leur IA distincte et complémentaire.

---

###  RedGhost

- Suit le joueur à la trace  
- Se dirige systématiquement vers la **position exacte occupée par le joueur** sur la grille  
- Comportement direct et agressif  
- Occupe le **coin Haut-Droite** de la carte  

---

###  YellowGhost

- Cherche à **intercepter le joueur**  
- Vise une position située **4 cases devant la direction actuelle du joueur**  
- Anticipe les déplacements plutôt que de suivre directement  
- Occupe le **coin Haut-Gauche** de la carte  

---

###  GreenGhost

- Alterne entre **traque et fuite**, ce qui le rend imprévisible  
- Si la distance avec le joueur est **supérieure à 8 cases** → il poursuit le joueur  
- Si la distance est **inférieure à 8 cases** → il bat en retraite vers son coin  
- Occupe le **coin Bas-Gauche** de la carte  

---

### BlueGhost

- IA la plus complexe : cherche à **encercler le joueur**  
- Identifie la case située **2 cases devant Pac-Man**  
- Trace un vecteur allant de la position du **RedGhost** jusqu’à cette case  
- Double ensuite la longueur de ce vecteur pour déterminer sa cible finale  
- Occupe le **coin Bas-Droite** de la carte

##  Améliorations envisagées

Plusieurs améliorations auraient pu être ajoutées :
 
- Création de plusieurs niveaux avec difficulté progressive.
- Ajout d’un système de score.
- Ajout de sons et musiques.

---

### Compatibilité

 Le projet doit pouvoir être compilé sur les **environnements standards du cursus (Windows)**.

## Assets

**packmanPack**: 

- **PacMan.png**
- **greenGhost.png**
- **redghost.png**
- **yellowGhost.png**
- **blueGhost.png**
- **BigCoin.png**
- **Coin.png**

- Tous les assets sont libres de droits et se retouve sur le site itch.io
  https://vladpenn.itch.io/pacman

  ## Crédits

  -Hugo Leroyer
  
  -Hania Belaskri

  




















