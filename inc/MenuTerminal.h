#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cctype>
#include <optional>

#include "Header.h"

struct Option {
    std::string texte;
    char marker{ ':' };
    int offset{ 0 };
    bool numerique{ true };
};

void clearScreen();

class Menu{
protected:
    // Attributs demand�s
    std::string intitule;
    std::string  header;
    std::vector<Option> options;

    // Constantes de mise en forme
    size_t LARGEUR; //espaces
    const std::string BORDURE = "++" + std::string(LARGEUR, '=') + "++";
    const std::string VIDE = "||" + std::string(LARGEUR,' ') + "||";
    const std::string SEP = "||" + std::string(LARGEUR, '-') + "||";

    std::string texteEncadre(const std::string& text, const std::string& couleur = RESET, int offset = 0) const;

    std::vector<std::string> lignesHeader(const std::string& couleur = RESET) const;

    std::vector<std::string> lignesIntitule(const std::string& couleur = RESET) const;

    std::vector<std::string> stringOption(size_t idx, const std::string& couleur = RESET, bool mark = false, bool centered = true) const;

    int afficherCentre(const std::string& text, char marker = ':');

public:
    // Constructeur
    Menu(const std::string& i, const std::string& h = "", size_t l = 54): intitule(i), header(h), LARGEUR(l) {}

    // M�thode pour ajouter une option (valeur cha�ne)
    void ajouterOption(const Option& valeur) {options.push_back(valeur);}
};


class MenuRestreint : public Menu {
    std::vector<int> afficher(size_t choix, const std::string& couleur = RESET, const std::string& contraste = FONDBn);
public:
    int loop(const std::string& couleur = RESET, const std::string& contraste = FONDBn);

    MenuRestreint(const std::string& i, const std::string& h = "") : Menu(i,h){}
};

class MenuChamps : public Menu {
    std::vector<Coord> afficher(const std::string& couleur = RESET, const std::string& contraste = FONDBn);

public:

    std::vector<std::string> loop(const std::string& couleur = RESET, const std::string& contraste = FONDBn);

    MenuChamps(const std::string& i, const std::string& h="") : Menu(i,h){}
};



int consoleWidth();
char entreeListe(std::vector<char> list);

void setCursor(int x, int y);

bool entreeNum(std::string entree, int max = 0, int min = 0);
std::optional<std::string> nombreEchap(size_t max);
std::optional<std::string> texteEchap(size_t max = 999);

size_t visible(const std::string& s);