#include "Vue.h"
#include "MenuTerminal.h"
#include "Jeu.h"
#include "Console.h"
#include <iostream>
#include <cmath>
#include <cctype>
#include <algorithm>

void Vue::coup(Joueur& joueur, size_t choix, Coord pos) {
	joueur.poserTuile(selectTuile(choix), pos);
	retirerTuile(choix);
	joueur.payerPierres(choix);
}

//-----------------------------------------------------------------------------------------------
//                                           VueTerminal
//-----------------------------------------------------------------------------------------------

void VueTerminal::afficher(const Cite& c, Coord pos) const{
	int rayon{ 0 };
	for (const auto& pair : c.getPlateau()) {
		int x = pair.first.first;
		int y = pair.first.second;
		int r = std::ceil(std::sqrt(2 * x * x + 2 * y * y)) + 1;
		if (r > rayon) rayon = r;
	}
	for (int j = rayon; j >= -rayon; j--) {
		if (j > 0) std::cout << std::string(std::abs(j)*2, ' ');
		if (pos.first == -rayon && pos.second == j) std::cout << ">";
		else std::cout << " ";

		for (int i = -rayon; i <= rayon; i++) {
			if (std::abs(j + i) <= rayon) {
				Coord co{ i, j };
				if (!c.caseVide(co)) {
					const Case& cell = c.getPlateau().at(co);
					if (co == pos) std::cout << "\033[47m";
					else std::cout << toFond(cell.h);
					std::cout << *cell.hex;
				}
				else {
					if (co == pos) std::cout << "\033[47m";
					std::cout << ".."; 
					if (co == pos) std::cout << RESET;
				}
			}
			if (Coord({ i + 1, j }) == pos) std::cout << " >";
			else std::cout << "  ";
		}
		std::cout << "\n";
		if (j > 0) std::cout << std::string(std::abs(j)*2, ' ');
		std::cout << " ";
		for (int i = -rayon; i <= rayon; i++) {
			if (std::abs(j + i) <= rayon) {
				Coord co{ i, j };
				if (!c.caseVide(co)) {
					const Case& cell = c.getPlateau().at(co);
					int id = cell.id;
					if (co == pos) std::cout << "\033[47m";
					else std::cout << toFond(cell.h);
					std::cout << toColor(cell.hex->getCouleur());
					if (id < 10) std::cout << "0" << id;
					else std::cout << id;
					std::cout << RESET;
				}
				else {
					if (co == pos) std::cout << "\033[47m";
					std::cout << "..";
					if (co == pos) std::cout << RESET;
				}
			}
			std::cout << "  ";
		}
		std::cout << "\n";
	}
}

void VueTerminal::afficher(const Cite& c, const std::map<Coord, const Hex*>& overlay, int rt) const{
	int rayon{ 0 };
	for (const auto& pair : c.getPlateau()) {
		int x = pair.first.first;
		int y = pair.first.second;
		int r = std::ceil(std::sqrt(x * x + y * y));
		if (r > rayon) rayon = r;
	}
	rayon += rt;

	std::cout << "\nAffichage du plateau :\n\n";
	for (int j = rayon; j >= -rayon; j--) {
		if (j > 0) std::cout << std::string(std::abs(j)*2, ' ');
		for (int i = -rayon; i <= rayon; i++) {
			if (std::abs(j + i) <= rayon) {
				Coord co{ i, j };
				const Hex* h = nullptr;
				auto it = overlay.find(co);
				if (it != overlay.end()) {
					std::cout << "\033[48;5;255m";
					h = it->second;
				}
				else if (!c.caseVide(co)) {
					h = c.getPlateau().at(co).hex;
					std::cout << toFond(c.getPlateau().at(co).h);
				}
				if (h) {
					std::cout << *h;
				}
				else {
					std::cout << "..";
				}
			}
			std::cout << "  ";
		}
		std::cout << "\n";
		if (j > 0) std::cout << std::string(std::abs(j)*2, ' ');
		for (int i = -rayon; i <= rayon; i++) {
			if (std::abs(j + i) <= rayon) {
				Coord co{ i, j };
				const Hex* h = nullptr;
				auto it = overlay.find(co);
				if (it != overlay.end()) {
					h = it->second;
				}
				else if (!c.caseVide(co)) {
					h = c.getPlateau().at(co).hex;
					std::cout << toFond(c.getPlateau().at(co).h);
				}
				if (h) {
					if (!c.caseVide(co)) {
						int id = c.getPlateau().at(co).id;
						if (id < 10) std::cout << toColor(h->getCouleur()) << "0" << id << RESET;
						else std::cout << toColor(h->getCouleur()) << id << RESET;
					}
					else {
						std::cout<<RESET<<toColor(h->getCouleur())<< "\033[48;5;255m" << "##"<<RESET;
					}
				}
				else {
					std::cout << "..";
				}
			}
			std::cout << "  ";
		}
		std::cout << "\n";
	}
}

void VueTerminal::afficherChantier(const std::vector<TuilePtr>& c) const {
	std::cout << "Nombre de piles restantes: " << partie->cartesRestantes()/(partie->getNbJoueurs()+1)<<"\n";
	std::vector<std::vector<std::string>> tl;
	for (const auto& t : c) {
		tl.emplace_back(t->toLignes());
	}

	int maxl=0;
	std::vector<size_t> widths(tl.size(),0);
	for (int i = 0; i < tl.size();i++) {
		for (const auto& l : tl[i]) {
			if (widths[i] < visibleLength(l)) widths[i] = visibleLength(l);
		}
		if (tl[i].size() > maxl) maxl = tl[i].size();

		std::cout << "Cout: " << i;
		size_t padding = 4;
		if (widths[i] > 7) padding += widths[i] - 7;
		std::cout << std::string(padding, ' ');
	}
	std::cout << "\n";
	for (int i = 0; i < maxl;i++) {
		for (int j = 0; j < tl.size(); j++) {
			size_t padding = 4;
			if (widths[j] < 7) padding += 7 - visibleLength(tl[j][i]);
			else if (i < tl[j].size()) padding += widths[j] - visibleLength(tl[j][i]);

			if (i < tl[j].size()) std::cout << tl[j][i] << std::string(padding, ' ');
			else std::cout << std::string(padding, ' ');
		}
		std::cout << "\n";
	}
}

void VueTerminal::afficher(const std::vector<TuilePtr>& c) const{
	std::vector<std::vector<std::string>> tl;
	for (const auto& t : c) {
		tl.emplace_back(t->toLignes());
	}
	int maxl = 0;
	std::vector<size_t> widths(tl.size(), 0);
	for (int i = 0; i < tl.size(); i++) {
		for (const auto& l : tl[i]) {
			if (widths[i] < visibleLength(l)) widths[i] = visibleLength(l);
		}
		if (tl[i].size() > maxl) maxl = tl[i].size();
		size_t padding = 4;
		if (widths[i] > 7) padding += widths[i] - 7;
	}
	for (int i = 0; i < maxl; i++) {
		for (int j = 0; j < tl.size(); j++) {
			size_t padding = 4;
			if (widths[j] < 7) padding += 7 - visibleLength(tl[j][i]);
			else if (i < tl[j].size()) padding += widths[j] - visibleLength(tl[j][i]);

			if (i < tl[j].size()) std::cout << tl[j][i] << std::string(padding, ' ');
			else std::cout << std::string(padding, ' ');
		}
		std::cout << "\n";
	}
}

void VueTerminal::afficherChantier(const std::vector<TuilePtr>& c, size_t index, Coord pivot) const {
	std::cout << "Nombre de piles restantes: " << partie->cartesRestantes() / (partie->getNbJoueurs() + 1) << "\n";
	std::vector<std::vector<std::string>> tl;
	for (size_t i = 0; i < c.size();i++) {
		if (i == index) tl.emplace_back(c[i]->toLignes(pivot));
		else tl.emplace_back(c[i]->toLignes());
	}
	int maxl = 0;
	std::vector<size_t> widths(tl.size(), 0);
	for (int i = 0; i < tl.size(); i++) {
		for (const auto& l : tl[i]) {
			if (widths[i] < visibleLength(l)) widths[i] = visibleLength(l);
		}
		if (tl[i].size() > maxl) maxl = tl[i].size();

		std::cout << "Cout: " << i;
		size_t padding = 4;
		if (widths[i] > 7) padding += widths[i] - 7;
		std::cout << std::string(padding, ' ');
	}
	std::cout << "\n";
	for (int i = 0; i < maxl; i++) {
		for (int j = 0; j < tl.size(); j++) {
			size_t padding = 4;
			if (widths[j] < 7) padding += 7 - visibleLength(tl[j][i]);
			else if (i < tl[j].size()) padding += widths[j] - visibleLength(tl[j][i]);

			if (i < tl[j].size()) std::cout << tl[j][i] << std::string(padding, ' ');
			else std::cout << std::string(padding, ' ');
		}
		std::cout << "\n";
	}
}

void VueTerminal::afficherIllustreArchitecte(const IllustreArchitecte& j) const{
	clearScreen();
	std::cout << "Tuiles de l'illustre architecte:\n";
	afficher(j.getTuiles());
	std::cout << "\n Appuyez sur entree pour continuer";
	entreeAttendue({ ENTREE });
}

void VueTerminal::afficherInfos(const JoueurHumain& joueur) const {
	std::cout << "\r";
	std::cout << "\nTour de " << joueur.getNom();
	std::cout << "\n Scores:\n  ";
	for (const auto& j : partie->getJoueursRead()) std::cout << j->getNom() << ": " << j->getScore() << std::string(5, ' ');
	std::cout << "\n\n Pierres: " << joueur.getPierres();
}

//-----------------------------------------------------------------------------------------------
//                                         Tour
//-----------------------------------------------------------------------------------------------

size_t VueTerminal::choixTuile(const JoueurHumain& joueur) const {
	size_t choix;
	while (true) {
		clearScreen();
		afficherInfos(joueur);
		std::cout << "\n Chantier:\n";
		afficherChantier(partie->getChantierRead());
		std::cout << "\n Cite: \n";
		afficher(joueur.getCite());
		std::cout << "\nChoix de la tuile: ";

		std::string entree;
		std::cin >> entree;

		if (entreeNumerique(entree, getChantierRead().size() - 1)) {
			choix = std::stoi(entree);
			if (choix <= joueur.getPierres()) return choix;
			clearScreen();
			std::cout << "Pas assez de pierres, appuyez sur Entree pour continuer";
			entreeAttendue({ ENTREE });
		}
	}
}

bool VueTerminal::choixPivot(const JoueurHumain& joueur, size_t index, Coord& pivot) {
	Coord save = pivot;
	const std::map<Coord, const Hex*>& t = partie->getChantierRead()[index]->getHexs();
	while (true) {
		clearScreen();
		afficherInfos(joueur);
		std::cout << "\n Chantier:\n";
		afficherChantier(partie->getChantierRead(), index, pivot);
		std::cout << "\n Cite: \n";
		afficher(joueur.getCite());

		std::cout << "\n Deplacez le pivot [z=Haut, s=Bas, q=Gauche, d=Droite, e=Valider, a=Annuler]: (" << pivot.first << " , " << pivot.second << ") ";

		char c = entreeAttendue({'z', 's', 'q', 'd', 'e', 'a', ECHAP, ENTREE});
		if (c == 'q') {
			if (t.find({ pivot.first - 1,pivot.second }) != t.end()) --pivot.first;
		}
		else if (c == 'd') {
			if (t.find({ pivot.first + 1,pivot.second }) != t.end()) ++pivot.first;
		}
		else if (c == 'z') {
			if (t.find({ pivot.first,pivot.second + 1 }) != t.end()) ++pivot.second;
		}
		else if (c == 's') {
			if (t.find({ pivot.first,pivot.second - 1 }) != t.end())  --pivot.second;
		}
		else if (c == 'e' || c == ENTREE) {
			setCentre(index, pivot);
			return true;
		}
		else if (c == 'a' || c == ECHAP) {
			pivot = save;
			setCentre(index, pivot);
			return false;
		}
	}
}

bool VueTerminal::choixCase(const JoueurHumain& joueur, Coord& pos, size_t index, Coord pivot){
	Coord save = pos;
	const Cite& cite = joueur.getCite();
	while (true) {
		clearScreen();
		afficherInfos(joueur);
		std::cout << "\n Chantier:\n\n";
		afficherChantier(partie->getChantierRead(), index, pivot);
		std::cout << "\n Cite: \n";
		afficher(joueur.getCite(), pos);

		std::cout << "\n Deplacez l'emplacement [z=Haut, s=Bas, q=Gauche, d=Droite, e=Valider, a=Annuler]: (" << pos.first << " , " << pos.second << ") ";

		char c = entreeAttendue({'z', 's', 'q', 'd', 'e', 'a', ECHAP, ENTREE});
		if (c == 'q') {
			if (cite.caseAdjacente({ pos.first - 1 , pos.second })) --pos.first;
		}
		else if (c == 'd') {
			if (cite.caseAdjacente({ pos.first + 1 , pos.second })) ++pos.first;
		}
		else if (c == 'z') {
			if (cite.caseAdjacente({ pos.first , pos.second + 1 })) ++pos.second;
		}
		else if (c == 's') {
			if (cite.caseAdjacente({ pos.first , pos.second - 1 }))  --pos.second;
		}
		else if ((c == 'e' || c == ENTREE) && cite.placementPossible(partie->getChantier()[index], pos)) {
			return true;
		}
		else if (c == 'a' || c == ECHAP) {
			pos = save;
			return false;
		}
	}
}

bool VueTerminal::choixRotation(const JoueurHumain& joueur, Coord pos, size_t index, Coord pivot) {
	const Cite& cite = joueur.getCite();
	while (true) {
		clearScreen();
		afficherInfos(joueur);
		std::cout << "\n Chantier:\n\n";
		afficherChantier(partie->getChantierRead(), index, pivot);
		std::cout << "\n Cite: \n";
		afficher(cite, partie->getChantierRead()[index]->getCoordPlateau(pos), partie->getChantierRead()[index]->rayon());

		std::cout << "\n Choisissez la rotation [q=Gauche, d=Droite, e=Valider, a=Annuler]";

		char c = entreeAttendue({'q', 'd', 'e', 'a', ECHAP, ENTREE});
		if (c == 'q') {
			partie->getChantier()[index]->rotation(true);
			while (!cite.coupLegal(partie->getChantierRead()[index], pos)) partie->getChantier()[index]->rotation(true);
		}
		else if (c == 'd') {
			partie->getChantier()[index]->rotation(false);
			while (!cite.coupLegal(partie->getChantierRead()[index], pos)) partie->getChantier()[index]->rotation(false);
		}
		else if (c == 'e' || c == ENTREE) return true;
		else if (c == 'a' || c == ECHAP) return false;
	}
}

void VueTerminal::jouer() {
	while (!partie->piocheVide()) {
		clearScreen();
		std::cout << "\n Tour " << partie->tour << "\nAppuyez sur Entree pour continuer";
		entreeAttendue({ ENTREE });
		partie->jouerTour(*this);
	}
	std::cout << "\nFin de la partie\n";
}

void VueTerminal::tour(JoueurHumain& joueur) {
	bool played = false;
	Coord pivot = { 0,0 };
	Coord pos = { 0,0 };

	while (!played) {
		size_t choix = choixTuile(joueur);

		if (!choixPivot(joueur, choix, pivot)) continue;
		if (!choixCase(joueur, pos, choix, pivot)) continue;
		if (!choixRotation(joueur, pos, choix, pivot)) continue;
		
		played = true;
		coup(joueur, choix, pos);
	}

	clearScreen();
	std::cout << "Tuile posee avec succes!\nAppuyez sur Entree pour continuer";
	entreeAttendue({ ENTREE });
}

//-----------------------------------------------------------------------------------------------
//                                           Construction
//-----------------------------------------------------------------------------------------------

std::vector<JoueurPtr> VueTerminal::creerJoueurs(const std::string& couleur, const std::string& contraste) {
	size_t nbJoueurs=999;
	std::vector<JoueurPtr> joueurs;

	MenuChamps menuJ = MenuChamps("JOUEURS","AKROPOLIS");
	menuJ.ajouterOption({ "Entrez le nombre de joueurs" }); 
	while (nbJoueurs > 4) {
		std::vector<std::string> entrees = menuJ.loop();
		if (entrees.empty()) return{};
		nbJoueurs = std::stoi(entrees[0]);
		if (nbJoueurs > 4) {
			clearScreen();
			std::cout << "Entree invalide, appuyez sur ENTREE pour continuer.";
			entreeAttendue({ ENTREE });
		}
	}

	MenuChamps menuN = MenuChamps("JOUEURS","AKROPOLIS");
	for (int i = 1; i <= nbJoueurs; i++) {
		menuN.ajouterOption({ "Entrez le nom du joueur " + std::to_string(i),':',0,false});
	}

	std::vector<std::string> noms = menuN.loop();
	size_t i{ 1 };
	for (const auto& nom : noms) {
		joueurs.emplace_back(JoueurPtr(new JoueurHumain(nom, i)));
		i++;
	}

	if (nbJoueurs == 1) {
		MenuRestreint menuD = MenuRestreint("SELECTION DE LA DIFFICULTE", "AKROPOLIS");
		menuD.ajouterOption({ "Hippodamos" });
		menuD.ajouterOption({ "Metagenes" });
		menuD.ajouterOption({ "Callicrates" });
		size_t choix = menuD.loop();
		NiveauDifficulte d = NiveauDifficulte::Hippodamos;
		switch (choix) {
		case 1: d = NiveauDifficulte::Hippodamos; break;
		case 2: d = NiveauDifficulte::Metagenes; break;
		case 3: d = NiveauDifficulte::Callicrates; break;
		}
		joueurs.emplace_back(JoueurPtr(new IllustreArchitecte(d)));
	}
	
	clearScreen();
	for(auto& j : joueurs) {
        j->initCite();
    }

	return joueurs;
}

std::vector<ReglePtr> VueTerminal::creerRegles(const std::string& couleur, const std::string& contraste) {
	std::vector<ReglePtr> regles;
	for (const auto& c : Couleurs) {
		if (c == Couleur::blanc) continue;
		size_t choix = 1;

		MenuRestreint menuR = MenuRestreint("SELECTION DES REGLES :"+toString(c), "AKROPOLIS");
		menuR.ajouterOption({ "Standard" });
		menuR.ajouterOption({ "Variante" });
		choix = menuR.loop();
		switch (choix) {
		case 1: regles.emplace_back(RegleFactory::creer(c, false)); break;
		case 2: regles.emplace_back(RegleFactory::creer(c, true)); break;
		default: return{};
		}
	}
	return regles;
}

VueTerminal::VueTerminal(const std::string& couleur, const std::string& contraste){
	std::vector<JoueurPtr> x = creerJoueurs(couleur,contraste);
	if (x.empty()) return;

	std::vector<ReglePtr> y = creerRegles(couleur,contraste);
	if (y.empty()) return;

	bool longue = false;
	if (x.size() < 4) {
		MenuRestreint menuL = MenuRestreint("MODE LONG", "AKROPOLIS");
		menuL.ajouterOption({ "Oui" });
		menuL.ajouterOption({"Non"});
		switch (menuL.loop()) {
		case 1: longue = true; break;
		case 2: longue = false; break;
		default: return;
		}
	}

	partie = std::unique_ptr<Partie>(new Partie(x,y,longue));
	jouer();
}

// -------------------------------------------------------------------------------------------------
//												MENU PRINCIPAL
// -------------------------------------------------------------------------------------------------

void header(const std::string& couleur) {
	std::cout << "\n";
	afficherCentre("++======================================================++",couleur);
	afficherCentre("||                                                      ||",couleur);
	afficherCentre("||                AKROPOLIS VERSION TERMINAL            ||", couleur);
	afficherCentre("||                                                      ||",couleur);
	afficherCentre("++======================================================++", couleur);
	std::cout<<RESET;
}

void menuPrincipalTerminal(const std::string& couleur, const std::string& contraste) {
	size_t choix = 1;
	MenuRestreint menuprincipal = MenuRestreint("MENU PRINCIPAL", "AKROPOLIS VERSION TERMINAL");
	menuprincipal.ajouterOption({ "Nouvelle Partie" });
	menuprincipal.ajouterOption({ "Charger une Partie" });
	menuprincipal.ajouterOption({ "Explication" });
	menuprincipal.ajouterOption({ "Quitter" });
	while (choix != 4) {
		choix = menuprincipal.loop();
		switch (choix) {
		case 1: nouvellePartie(); break;
		case 2: break;
		case 3: explications(); break;
		case 4: return;
		}
	}
}

void explications(const std::string& couleur) {
	clearScreen();
	header(couleur);
	std::cout << "\n";
	afficherCentre("Appuyez sur Echap (ou tapez 'a') pour revenir au menu principal",couleur);
	entreeAttendue({ECHAP, 'a'});
}

void nouvellePartie(const std::string& couleur,const std::string& contraste) {
	VueTerminal p(couleur,contraste);
}

//-----------------------------------------------------------------------------------------------
//                                           UTILITAIRE
//-----------------------------------------------------------------------------------------------

int largeurConsole() {
    return Console::getWidth();
}

void afficherCentre(const std::string& text, const std::string& couleur) {
	int width = largeurConsole();
	int padding = (width - text.size()) / 2;
	if (padding > 0)
		std::cout << std::string(padding, ' ');
	std::cout << couleur << text << RESET << std::endl;
}

void setCursorPosition(int x, int y) {
    Console::setCursor(x, y);
}

int afficherCentreAvecCurseur(const std::string& text, char stopAt, const std::string& couleur) {
	int width = largeurConsole();
	int padding = (width - text.size()) / 2;
	if (padding < 0) padding = 0;

	std::cout << std::string(padding, ' ') << couleur << text << "\n";

	int relativePos = text.find(stopAt);
	if (relativePos == std::string::npos)
		return -1;

	return padding + relativePos + 1; 
}

char entreeAttendue(std::vector<char> list) {
    return Console::readKeyFromList(list);
}

bool entreeNumerique(std::string entree, int max, int min) {
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
			entreeAttendue({ ENTREE });
		}
	}
	else {
		clearScreen();
		std::cout << "Entree invalide, appuyez sur Entree pour continuer";
		entreeAttendue({ ENTREE });
	}
	return valide;
}

std::optional<std::string> lireNombreAvecEchap(size_t max) {
    return Console::readLineWithEscape(max, true);
}

std::optional<std::string> lireTexteAvecEchap(size_t max) {
    return Console::readLineWithEscape(max, false);
}

size_t visibleLength(const std::string& s) {
    return visible(s);
}