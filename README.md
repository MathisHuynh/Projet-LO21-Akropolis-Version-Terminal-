# Akropolis-LO21
## Description:
  Dans ce projet, il s'agit de concevoir et développer une application permettant de jouer au jeu de société Akropolis créé par Jules Messaud, et édité par Gigamic. Dans ce jeu de pose de tuiles, les joueurs prennent le rôle d'architectes qui s'a rontent en créant chacun une cité à l'aide de tuiles cité . Chaque tuile cité est composé de 3 hexagones construction , chaque hexagone représentant un quartier , une place ou une carrière . Il existe plusieurs types de quartier.

## Règles:
### Quartiers:
Chaque quartier rapporte des points de victoire s'ils sont correctement placés selon sont type :

- les habitations (quartiers bleus) : seules les habitations faisant partie du plus grand groupe d'habitations adjacentes font gagner des points;

- les marchés (quartiers jaunes) : ils ne doivent pas être adjacents à un autre marché pour rapporter des points;

- les casernes (quartiers rouges) : elles doivent se trouver en périphérie de la cité du joueur pour rapporter des points;

- les temples (quartiers violets) : ils doivent être complétement entourés pour raporter des points;

- les jardins (tuiles vertes) : ils n'ont pas de contrainte de placement pour rapporter des points.

- Les carrières ne rapportent pas de point lors du décompte final, mais permettent d'obtenir des pierres qui rapportent des points à la fin ou permettent d'acheter des facilités de choix des tuiles cité lors de son tour.
	
- Les places sont des multiplicateurs, qui augmentent le nombre de points rapportés par les quartiers de même type. Le multiplicateur est représenté par des étoiles. Si un joueur possède plusieurs places de même type, leurs étoiles sont cumulables. Il n'est pas nécessaire qu'une place soit adjacente aux quartiers de même type.

### Pierres:
Quand un Architecte recouvre une carrière à l'aide d'une nouvelle tuile, il prend une pierre de la réserve. Les pierres permettent d'acheter des tuiles dans le chantier dont le prix est déterminé par leur position dans celui-ci.

### Déroulé du tour:
À chaque tour, les joueurs vont choisir une tuile cité selon des contraintes données et l'ajouter à leur cité de manière adjacente aux autres. Une tuile cité peut être posée au niveau du sol (niveau 1) ou la superposer sur des tuiles existantes (niveaux supérieurs).

Le jeu se déroule en plusieurs tours jusqu'à épuisement des tuiles cité dont le nombre dépend du nombre de joueurs.

Le joueur qui possède le plus de point de victoire à la n de la partie l'emporte. Ce jeu se joue de 2 à 4 joueurs mais propose un mode solo avec des règles précises simulant un adversaire virtuel appelé Illustre Constructeur.

Trois niveaux de difficulté vous sont proposés, faisant varier la façon dont les points adverses sont comptabilisés. Le jeu propose quelques variantes qui permettent de corser le jeu. Dans ce cas, la mise en place et le déroulé de la partie ne changent pas.

Chaque variante propose une condition de placement supplémentaire pour rapporter des points. Il est possible de jouer avec plusieurs variantes dans la même partie.

## Instructions de compilation:
Le projet a été conçu en grande partie sur Visual Studio et a été compilé sur Windows 11 avec CMake en utilisant MinGW g++.

Il est donc préférable d'installer CMake: https://cmake.org/download/

Ainsi que mingw-w64 GCC: https://www.msys2.org/

Commande de compilation Windows:
```
mkdir build; cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```
Commande de compilation Linux/macOS:
```
mkdir build && cd build
cmake ..
cmake --build .
```
