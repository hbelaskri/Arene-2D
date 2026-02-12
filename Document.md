# Mini-arène 2D – FSM et IA

## 1. Thème choisi
**Thème libre : Pacman**  

Nous avons choisi Pacman comme thème car il illustre parfaitement l’utilisation de FSM et d’IA :  

- Les fantômes sont des agents pilotés par une FSM avec différents états (Chase, Scatter, Frightened)  
- Le joueur est contrôlé par le clavier et interagit avec la map et les agents  
- Le jeu nécessite de séparer la logique de décision des personnages de l’animation et du rendu graphique  

Ce choix nous permet de mettre en pratique les concepts théoriques de FSM et de l’IA.

### Objectif du jeu
- Le joueur contrôle un personnage qui se déplace dans un labyrinthe.

**But :**  
- Collecter tous les objets présents sur la map  
- Éviter les ennemis (Fantômes)  
- Survivre jusqu’à ce que tous les collectibles soient récupérés  
- Si le joueur est attrapé par un ennemi → la partie se termine (Game Over)

### Contrôles
- **Joueur :** Z (haut), Q (gauche), S (bas), D (droite)  
- **Menus :** navigation à la souris  

---

## 2. Fonctionnalités réalisées
- 4 PNJ avec comportements distincts via FSM (RedGhost, YellowGhost, GreenGhost, BlueGhost)  
- Map éditée à la main avec blocs SFML et sauvegardée dans `map.txt`  
- Gestion du joueur et collisions  
- Collectibles fonctionnels  

---

## 3. Fonctionnalités non réalisées
- Multiples niveaux  
- Système de score et de vie  
- Sons et musiques  

*Ces fonctionnalités n’ont pas été ajoutées par manque de temps et priorité sur la FSM.*

---

## 4. Explication de la FSM

Chaque fantôme possède un comportement spécifique implémenté via une FSM, ce qui rend leur IA distincte et complémentaire.

### RedGhost
- Suit le joueur à la trace  
- Se dirige systématiquement vers la position exacte occupée par le joueur sur la grille  
- Comportement direct et agressif  
- Occupe le coin Haut-Droite de la carte  

### YellowGhost
- Cherche à intercepter le joueur  
- Vise une position située 4 cases devant la direction actuelle du joueur  
- Anticipe les déplacements plutôt que de suivre directement  
- Occupe le coin Haut-Gauche de la carte  

### GreenGhost
- Alterne entre traque et fuite, ce qui le rend imprévisible  
- Si la distance avec le joueur est supérieure à 8 cases → il poursuit le joueur  
- Si la distance est inférieure à 8 cases → il bat en retraite vers son coin  
- Occupe le coin Bas-Gauche de la carte  

### BlueGhost
- IA la plus complexe : cherche à encercler le joueur  
- Identifie la case située 2 cases devant Pac-Man  
- Trace un vecteur allant de la position du RedGhost jusqu’à cette case  
- Double ensuite la longueur de ce vecteur pour déterminer sa cible finale  
- Occupe le coin Bas-Droite de la carte  

---

## 5. Liste des assets externes utilisés
**packmanPack :**  
- PacMan.png  
- GreenGhost.png  
- RedGhost.png  
- YellowGhost.png  
- BlueGhost.png  
- BigCoin.png  
- Coin.png  

Tous les assets sont libres de droits et se retrouvent sur le site [itch.io](https://vladpenn.itch.io/pacman)  
https://vladpenn.itch.io/pacman

---

## 6. Crédits
- Hugo Leroyer  
- Hania Belaskri
