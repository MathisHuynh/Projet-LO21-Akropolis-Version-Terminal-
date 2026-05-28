#include "Console.h"
#include <iostream>
#include <algorithm>
#include <cctype>

#if defined(_WIN32)
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>

    // Mode non-canonique POSIX pour lecture caractere par caractere instantanee sous Linux/macOS
    static char getch_linux() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0) return 0;
        old.c_lflag &= ~ICANON; // Desactive la mise en memoire tampon par ligne
        old.c_lflag &= ~ECHO;   // Desactive l'affichage automatique du caractere saisi
        old.c_cc[VMIN] = 1;     // Attend au moins 1 caractere
        old.c_cc[VTIME] = 0;    // Pas de delai d'attente
        if (tcsetattr(0, TCSANOW, &old) < 0) return 0;
        if (read(0, &buf, 1) < 0) buf = 0;
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        tcsetattr(0, TCSAD_FILES, &old); // Restaure les parametres d'origine du terminal
        return buf;
    }
#endif

namespace Console {

    void clear() {
        std::cout << "\033[2J\033[1;1H" << std::flush;
    }

    void setCursor(int x, int y) {
#if defined(_WIN32)
        COORD pos = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
#endif
    }

    int getWidth() {
#if defined(_WIN32)
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
#endif
        return 80; // Largeur par defaut sous Unix si non detectee
    }

    char readKeyFromList(const std::vector<char>& listeValide) {
        while (true) {
#if defined(_WIN32)
            char c = _getch();
#else
            char c = getch_linux();
#endif
            if (std::find(listeValide.begin(), listeValide.end(), c) != listeValide.end()) {
                return c;
            }
        }
    }

    std::optional<std::string> readLineWithEscape(size_t maxChars, bool numeriqueUniquement) {
        std::string entree;

        while (true) {
            char c = 0;

#if defined(_WIN32)
            int val = _getch();
            if (val == 0 || val == 224) { _getch(); continue; } // Ignore les touches etendues flèche/F1-F12
            c = static_cast<char>(val);
#else
            c = getch_linux();
#endif

            if (c == 27) return std::nullopt; // Code de la touche Echap
            
            if (c == 13 || c == '\n' || c == '\r') { // Touches d'entree / validation
                if (!entree.empty()) {
                    std::cout << '\n' << std::flush;
                    return entree;
                }
            }
            else if (c == 8 || c == 127) { // Effacement arriere (Windows: 8, Linux/macOS: 127)
                if (!entree.empty()) {
                    entree.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
            }
            else if (entree.size() < maxChars) {
                if (!numeriqueUniquement || std::isdigit(static_cast<unsigned char>(c))) {
                    entree.push_back(c);
                    std::cout << c << std::flush;
                }
            }
        }
    }
}