#include "MenuTerminal.h"
#include <windows.h>

void clearScreen(){
    std::cout << "\033[2J\033[1;1H" << std::flush;
}


//================================================================
//                          MENU
//================================================================

// RAPPEL : LARGEUR = 54 (longueur interne entre les barres verticales)
// VIDE = "||                                                      ||" (longueur totale 58)

std::string Menu::texteEncadre(const std::string& text, const std::string& couleur, int offset) const {
    size_t s = visible(text);
    size_t paddingG = (LARGEUR - s) / 2;
    // Si la largeur visible est sup�rieure � la LARGEUR disponible, on n'ajoute pas de padding.
    if (s > LARGEUR) paddingG = 0;
    const size_t marge = 2; // Index apr�s les deux barres "||"
    size_t start = marge + paddingG + offset;
    std::string vide = VIDE;
    std::string res;
    res += couleur;
    res += vide.substr(0, marge); // Ajoute "||" (Index 0 et 1)
    res += std::string(paddingG + offset, ' ');
    res += text; // text contient la couleur si elle change l'apparence du texte lui-m�me

    size_t paddingD = LARGEUR - (paddingG + s + offset);
    if (paddingD > 0) {
        res += std::string(paddingD, ' ');
    }
    res += vide.substr(marge + LARGEUR);
    res += RESET;

    return res;
}

std::vector<std::string> Menu::lignesHeader(const std::string& couleur) const {
    std::vector<std::string> res;
    res.push_back(couleur + BORDURE + RESET);
    res.push_back(couleur + VIDE + RESET);
    res.push_back(texteEncadre(header, couleur));
    res.push_back(couleur + VIDE + RESET);
    res.push_back(couleur + BORDURE + RESET);
    return res;
}

std::vector<std::string> Menu::lignesIntitule(const std::string& couleur) const {
    std::vector<std::string> res;
    res.push_back(texteEncadre(">>" + intitule + "<<", couleur));
    res.push_back(couleur + VIDE + RESET);
    res.push_back(couleur + BORDURE + RESET);
    return res;
}

std::vector<std::string> Menu::stringOption(size_t idx, const std::string& couleur, bool mark, bool centered) const {
    std::vector<std::string> res;
    if (mark) {
        if(centered) res.push_back(texteEncadre(options[idx].texte + options[idx].marker, couleur, options[idx].offset));
        else {
            size_t s = visible(options[idx].texte + options[idx].marker);
            int paddingG = (LARGEUR - s) / 2;
            res.push_back(texteEncadre(options[idx].texte + options[idx].marker, couleur,-paddingG));
        }
    }


    else {
        if(centered) res.push_back(texteEncadre('[' + std::to_string(idx + 1) + "] " + options[idx].texte, couleur, options[idx].offset));
        else {
            size_t s = visible('[' + std::to_string(idx + 1) + "] " + options[idx].texte);
            int paddingG = (LARGEUR - s) / 2;
            res.push_back(texteEncadre('[' + std::to_string(idx + 1) + "] " + options[idx].texte, couleur, -paddingG));
        }
    }


    if (idx != options.size() - 1) res.push_back(couleur + SEP + RESET);
    else res.push_back(couleur + BORDURE + RESET);
    return res;
}

int Menu::afficherCentre(const std::string& text, char marker) {
    int width = consoleWidth();
    int padding = (width - visible(text)) / 2;
    if (padding < 0) padding = 0;

    std::cout << std::string(padding, ' ') << text << "\n";

    // Trouver la position du caract�re o� placer le curseur
    int relativePos = text.find(marker);
    if (relativePos == std::string::npos)
        return -1;

    // Position console = marge + position dans le texte
    return padding + relativePos + 1;
}


//================================================================
//                          MENU RESTREINT
//================================================================



std::vector<int> MenuRestreint::afficher(size_t choix, const std::string& couleur, const std::string& contraste) {
    for (const auto& ligne : lignesHeader(couleur)) afficherCentre(ligne);
    for (const auto& ligne : lignesIntitule(couleur)) afficherCentre(ligne);
    std::vector<int> res;
    for (size_t i = 0; i < options.size(); i++) {
        std::vector<std::string> op;
        op = stringOption(i, couleur);
        if (choix == i + 1) op[0] = stringOption(i, contraste)[0];
        res.push_back(afficherCentre(op[0], options[i].marker));
        afficherCentre(op[1]);
    }
    return res;
}

int MenuRestreint::loop(const std::string& couleur, const std::string& contraste) {
    if (options.empty()) throw JeuException("Erreur: le menu n'a pas d'option disponible");
    char entree{ 0 };
    int choix{ 1 };
    while (entree != ENTREE) {
        clearScreen();
        afficher(choix, couleur, contraste);
        std::vector<char> liste = { ECHAP,ENTREE,KEY_UP,KEY_DOWN };
        for (size_t i = 1; i <= options.size(); i++) liste.push_back(i + '0');

        entree = entreeListe(liste);
        switch (entree) {
        case ECHAP: return -1;
        case ENTREE: break;
        case KEY_UP: choix = (choix - 1) % options.size();
            break;
        case KEY_DOWN: choix = (choix + 1) % options.size();
            break;
        default: choix = entree - '0';
        }
        if (choix == 0) choix = options.size();
    }
    return choix;
}

//================================================================
//                          MENU CHAMPS
//================================================================

std::vector<Coord> MenuChamps::afficher( const std::string& couleur, const std::string& contraste) {
    for (const auto& ligne : lignesHeader(couleur)) afficherCentre(ligne);
    for (const auto& ligne : lignesIntitule(couleur)) afficherCentre(ligne);
    std::vector<Coord> res;
    for (size_t i = 0; i < options.size(); i++) {
        std::vector<std::string> op;
        op = stringOption(i, couleur,true,false);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int y = csbi.dwCursorPosition.Y;

        res.push_back(Coord(afficherCentre(op[0], options[i].marker),y));
        afficherCentre(op[1]);
    }
    return res;
}

std::vector<std::string> MenuChamps::loop(const std::string& couleur, const std::string& contraste) {
    //if (options.empty()) throw JeuException("Erreur: le menu n'a pas d'option disponible");
    std::vector<std::string> res = {};
    bool entree = false;
    while (!entree) {
        res.clear();
        clearScreen();
        std::vector<Coord> Pos = afficher(couleur, contraste);
        for (size_t i = 0 ; i < options.size(); i++) {
            entree = false;
            setCursor(Pos[i].first-4, Pos[i].second);
            if (options[i].numerique) {
                auto entreeR = nombreEchap(26); //changer
                if (!entreeR) {
                    clearScreen();
                    if (i == 0) return {};
                    else break;
                }
                res.push_back(*entreeR);
                entree = true;
            }
            else {
                auto entreeR = texteEchap(26); //changer
                if (!entreeR) {
                    clearScreen();
                    if (i == 0) return {};
                    else break;
                }
                res.push_back(*entreeR);
                entree = true;
            }
        }
    }
    clearScreen();
    return res;
}


//================================================================
//                          UTILITAIRE
//================================================================

int consoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

char entreeListe(std::vector<char> list) {
    char c = _getch();
    while (std::find(list.begin(), list.end(), c) == list.end()) c = _getch();
    return c;
}

void setCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


bool entreeNum(std::string entree, int max, int min) {
    bool valide = false;
    bool digit = true;
    for (const char& c : entree) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            digit = false;
            break;
        }
    }
    if (digit) {
        int n = std::stoi(entree);
        if (n >= min && n <= max) {
            valide = true;
        }
        else {
            clearScreen();
            std::cout << "Entree invalide, appuyez sur Entree pour continuer";
            entreeListe({ ENTREE });
        }
    }
    else {
        clearScreen();
        std::cout << "Entree invalide, appuyez sur Entree pour continuer";
        entreeListe({ ENTREE });
    }
    return valide;
}

std::optional<std::string> nombreEchap(size_t max) {
    std::string entree;
    while (true) {
        int c = _getch(); //int pour les codes �tendus (fleches etc.)
        // Gestion des touches sp�ciales (Fl�ches, F1..., etc.)
        if (c == 0 || c == 224) {
            _getch(); // On prend le deuxi�me code pour l'ignorer
            continue;
        }
        if (c == ECHAP) return std::nullopt;
        else if (c == ENTREE) {
            if (!entree.empty()) {
                std::cout << '\n';
                return entree;
            }
        }
        else if (c == '\b') { // Backspace
            if (!entree.empty()) {
                entree.pop_back();
                std::cout << "\b \b";
            }
        }
        else if (std::isdigit(static_cast<unsigned char>(c)) && entree.size() < max) {
            entree.push_back(static_cast<char>(c));
            std::cout << static_cast<char>(c);
        }
    }
}


std::optional<std::string> texteEchap(size_t max) {
    std::string entree;
    while (true) {
        int c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c == ECHAP) return std::nullopt;
        else if (c == ENTREE) {
            if (!entree.empty()) {
                std::cout << '\n';
                return entree;
            }
        }
        else if (c == '\b') {
            if (!entree.empty()) {
                entree.pop_back();
                std::cout << "\b \b";
            }
        }
        // On accepte les caract�res affichables
        else if (c >= 32 && c <= 255 && entree.size() < max) {
            entree.push_back(static_cast<char>(c));
            std::cout << static_cast<char>(c);
        }
    }
}

size_t visible(const std::string& s) {
    size_t len = 0;
    for (size_t i = 0; i < s.size(); ) {
        if (s[i] == '\x1b' && i + 1 < s.size() && s[i + 1] == '[') {
            // on est dans une s�quence d'�chappement CSI : \x1b[ ... jusqu'� 'm'
            i += 2; // skip \x1b[
            while (i < s.size() && s[i] != 'm') ++i;
            if (i < s.size()) ++i; // skip final 'm'
        }
        else {
            ++len;
            ++i;
        }
    }
    return len;
}