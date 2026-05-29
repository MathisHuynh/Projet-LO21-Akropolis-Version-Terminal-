# Akropolis (LO21)

> Une adaptation en C++ moderne et interface Terminal du célèbre jeu de société stratégique de pose de tuiles.

---

## Présentation du Projet

**Akropolis-LO21** est une implémentation logicielle du jeu de société éponyme créé par *Jules Messaud* et édité par *Gigamic*. Développé entièrement en **C++**, ce projet met en application les concepts fondamentaux de la **Conception et de la Programmation Orientées Objets (POO)**.

Ce projet a été réalisé dans le cadre de l'unité d'enseignement **LO21 - Programmation et Conception Orientées Objets** à l'**Université de Technologie de Compiègne (UTC)**.

Ce dépôt héberge l'architecture du **Moteur de Jeu** ainsi que son interface en ligne de commande (**Terminal**), entièrement conçus et programmés par ***Mathis HUYNH***, les modules Vue GUI (***Mathis HUYNH & Chloé LAMEZEC***) et la sauvegarde en serialization JS (***Chloé LAMEZEC***) sont dans les fichiers sources mais ne sont pas implémentés dans cette version.

Une version en terminal *xterm.js* est disponible à l'<a href="https://mathishuynh.github.io/Projet-LO21-Akropolis-Version-Terminal/">**ADRESSE SUIVANTE**</a>.

---

## Règles du Jeu & Mécaniques

Dans Akropolis, les joueurs incarnent des architectes rivalisant de talent pour bâtir la cité la plus prestigieuse au cœur de la Grèce antique. La construction repose sur l'assemblage de **tuiles Cité**, chacune composée de 3 hexagones représentant des quartiers, des places ou des carrières.

### Typologie des Quartiers & Scoring
Chaque type de quartier possède ses propres critères de placement pour générer des points de victoire lors du décompte final :

* **Habitations (Bleu) :** Seul le plus grand groupe d'habitations adjacentes est comptabilisé.
* **Marchés (Jaune) :** Doivent être isolés. Ils ne rapportent aucun point s'ils sont adjacents à un autre marché.
* **Casernes (Rouge) :** Doivent impérativement être situées en périphérie (sur les bords extérieurs) de la cité.
* **Temples (Violet) :** Doivent être totalement clos (entourés par 6 autres hexagones adjacents).
* **Jardins (Vert) :** Ne possèdent aucune contrainte de placement, ils scorent de manière inconditionnelle.
* **Carrières :** Ne rapportent pas de points directement, mais l'extraction de **pierres** sert de monnaie d'échange pour acquérir de futures tuiles dans le chantier.
* **Places (Multiplicateurs) :** Indispensables pour valoriser vos quartiers. Chaque place affiche des étoiles qui agissent comme un multiplicateur cumulable pour les quartiers de couleur correspondante. Il n'est pas nécessaire qu'une place soit adjacente aux quartiers associés pour s'appliquer.

### Gestion des Ressources : Les Pierres
Lorsqu'un architecte superpose une tuile et recouvre une carrière existante, il gagne une pierre de la réserve. Les pierres permettent d'acheter des tuiles plus lointaines dans le chantier de construction ou se convertissent en points bonus à la fin de la partie.

### Déroulement d'un Tour
Le jeu se déroule au tour par tour jusqu'à épuisement de la pioche de tuiles :
1. **Sélection :** Le joueur choisit une tuile dans le chantier (en dépensant des pierres selon sa position).
2. **Placement :** La tuile est ajoutée à la cité de manière adjacente. Elle peut être posée au sol (**Niveau 1**) ou superposée sur des tuiles existantes (**Niveaux supérieurs**), ce qui augmente la valeur des quartiers sous-jacents ou génère des pierres.

---

## Modes de Jeu & Variantes

* **Mode Solo :** Affrontez l'**Illustre Constructeur**, un adversaire virtuel géré par le moteur de jeu, déclinable en **3 niveaux de difficulté** qui ajustent son algorithme de calcul de points.
* **Variantes Officielles :** Le jeu intègre des règles avancées optionnelles ajoutant des conditions de placement supplémentaires pour corser la stratégie. Ces variantes sont cumulables au sein d'une même partie.

---

## Configuration & Compilation

Le projet utilise **CMake** comme système de génération de fichiers de build et est conçu pour être multiplateforme.

### Prérequis
* Un compilateur C++ compatible (ex: **GCC / g++**)
* **CMake** (version 3.10 ou supérieure recommandée)
* *Pour Windows :* L'environnement **MSYS2 (mingw-w64)** installé et configuré dans le PATH.
* *Pour compiler en Javascript :* Le SDK **Emscripten (emsdk)** installé, activé et chargé dans l'environnement.

### Commandes de Compilation

#### Windows (avec MinGW)
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```
#### Linux/macOS
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

#### Javascript (Web)
```bash
mkdir build-web
cd build-web
emcmake cmake -G "MinGW Makefiles" .. # ou emcmake cmake .. en Linux/macOS
cmake --build .
```
