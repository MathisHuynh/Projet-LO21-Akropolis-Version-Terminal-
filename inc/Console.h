#pragma once
#include <string>
#include <vector>
#include <optional>

namespace Console {
    void clear();
    void setCursor(int x, int y);
    int getWidth();
    char readKeyFromList(const std::vector<char>& listeValide);
    std::optional<std::string> readLineWithEscape(size_t maxChars, bool numeriqueUniquement);
}