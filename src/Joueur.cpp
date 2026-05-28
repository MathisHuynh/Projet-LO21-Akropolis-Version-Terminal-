#include "Joueur.h"
#include "Partie.h"
#include "Vue.h"

#include <cctype>

size_t IllustreArchitecte::choisirIndexTuile(const std::vector<TuilePtr>& chantier) const {
    size_t best = (size_t)-1;

    for (size_t i = 0; i < chantier.size(); ++i) {
        if (chantier[i]->contientPlace()) {
            if (best == (size_t)-1 || i < best)
                best = i;
        }
    }
    // Aucun Place -> prend la premi�re
    if (best == (size_t)-1) return 0;
    // Pas assez de pierres -> prend la premi�re
    if (best > pierres) return 0;
    return best;
}

void IllustreArchitecte::poserTuile(TuilePtr& t, Coord pos) {
    tuiles.push_back(std::move(t));
}

size_t IllustreArchitecte::scoreHorsCite() const {
    size_t scoret = 0;
    for (const auto& c : Couleurs) {
        if (c == Couleur::blanc && difficulte!=NiveauDifficulte::Metagenes) continue;
        size_t etoiles = 1;
        size_t scores = 0;
        for (const auto& t : tuiles) {
            for (const auto& h : t->getHexs()) {
                if (h.second->getCouleur() == c) {
                    etoiles += h.second->getNbEtoile();
                    if (h.second->getNbEtoile() == 0) {
                        if (difficulte == NiveauDifficulte::Callicrates) scores += 2;
                        else if (c != Couleur::blanc) scores += 1;
                        else scores += 2;
                    }
                }
            }
        }
        scoret += scores * etoiles;
    }
    return scoret+getPierres();
}

void JoueurHumain::jouerTour(Vue& vue) {
    vue.tour(*this);
}


void IllustreArchitecte::jouerTour(Vue& vue) {
    if (vue.chantierVide()) return;
    size_t idx = choisirIndexTuile(vue.getChantierRead());
    if (idx >= vue.tailleChantier()) idx = 0;
    size_t prix = idx;
    // Paiement
    if (prix <= pierres)
        pierres -= prix;

    vue.coup(*this, idx);
}