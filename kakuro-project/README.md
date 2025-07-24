#  Projet Kakuro – Solveur en C++

##  Objectif

Ce projet est un **résolveur de grilles Kakuro** développé en C++ en utilisant la **programmation orientée objet (POO)** et le **design pattern Factory**. Il permet de :
- Charger une grille de Kakuro à partir d’un fichier texte,
- Construire dynamiquement la structure de la grille,
- Appliquer un algorithme de **backtracking** pour résoudre le puzzle,
- Afficher la grille initiale et la solution.

---

##  Architecture du Code

### 1. `Cell` (classe abstraite)
Représente une cellule générique dans la grille. Trois types en héritent :
- `BlackCell` : case noire (non modifiable)
- `ClueCell` : case contenant les indices de somme (verticale et/ou horizontale)
- `PlayableCell` : cellule à remplir par un chiffre (1 à 9)

### 2. `KakuroGrid`
Représente la grille du jeu. Elle contient :
- Un tableau 2D de cellules (`grid`)
- Un vecteur de groupes (`Group`) définis à partir des indices (clues), chacun contenant :
    - La somme cible (`requiredSum`)
    - Un vecteur de pointeurs vers les cellules à remplir

### 3. `GridLoader` (interface abstraite)
Classe de base pour charger une grille :
- `SimpleGridLoader` : lit une grille depuis un format texte (`X`, `C`, `.`, chiffre)
- `AdvancedGridLoader` : lit une grille au format CSV (optionnel)
- `GridLoaderFactory` : applique le design pattern Factory pour retourner dynamiquement le bon type de loader

### 4. `Solver`
Classe de résolution du puzzle utilisant du **backtracking** :
- `solve()` tente toutes les combinaisons possibles dans les cellules jouables
- `isValidGroup()` vérifie les contraintes (somme exacte, chiffres uniques)
- `isValid()` valide toutes les contraintes de la grille

---

##  Exemple de fichier de grille (`puzzle.txt`)

