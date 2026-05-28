#include "Cite.h"
#include <algorithm>

const Coord Cite::adjacence[6]={
	{ 1, 0 }, //NE
	{ -1,0 }, //SO
	{ 1,-1 }, //NO
	{ -1,1 }, //SE
	{ 0,1 }, //E
	{ 0,-1 } //O
};

class Joueur;

void Cite::init() {
    placerTuile(TuileFactory::creerTuileDepart(), { 0,0 });
}

//-----------------------------------------------------------------------------------------------
//                                               BOOL
//-----------------------------------------------------------------------------------------------

bool Cite::caseVide(const Coord& pos) const {
	const auto& s = plateau.find(pos);
	return s == plateau.end();
}
bool Cite::caseAdjacente(const Coord& pos) const{
	for (auto d : adjacence) {
		if (!caseVide({ pos.first + d.first,pos.second + d.second})) return true;
	}
	return false;
}

int Cite::placerHex(const Hex* h, const Coord& pos, int id) {
	int n = 0;
	if (!caseVide(pos)) {
		plateau[pos].h++;
		plateau[pos].id = id;
		n = plateau[pos].hex->recouvert();
		plateau[pos].hex = h;
	}
	else {
		Case cell;
		cell.id = id;
		cell.hex = h;
		plateau[pos] = cell;
	}
	return n;
}

bool Cite::coupLegal(const TuilePtr& t, const Coord& pos) const {
	if (!caseAdjacente(pos) && !plateau.empty()) return false;
	size_t h{ 0 };
	int id{ 0 };
	bool chevauchement = false;

	if (!caseVide(pos)) {
		h = plateau.at(pos).h;
		id = plateau.at(pos).id;
	}

	for (auto& pair : t->getCoordPlateau(pos)) {
		if (caseVide(pair.first)) {
			if (h != 0) return false;
		}
		else {
			if (h != plateau.at(pair.first).h) return false;
			else if (id != plateau.at(pair.first).id) chevauchement = true;
		}
	}

	return (h==0 || chevauchement);
}


bool Cite::placementPossible(TuilePtr& t, const Coord& pos) const{
	for (size_t i = 0; i < 6; i++) {
		if (coupLegal(t, pos)) return true;
		t->rotation(true);
	}
	return false;
}

bool Cite::estCouleurIsoleSaufPlace(const Coord& pos) const {
	for (const Coord& c : casesAdjacentes(pos)) {
		if (plateau.at(pos).hex->getCouleur() == plateau.at(c).hex->getCouleur()) {
			if (plateau.at(c).hex->getNbEtoile() == 0)
				return false;
		}
	}
	return true;
}

bool Cite::adjacentAPlaceCouleur(const Coord& pos) const {
	Couleur c = plateau.at(pos).hex->getCouleur();
	for (auto d : Cite::adjacence) {
		Coord voisin{ pos.first + d.first,pos.second + d.second };
		if (!caseVide(voisin) && plateau.at(voisin).hex->getCouleur() == c && plateau.at(voisin).hex->getNbEtoile() != 0) return true;
	}
	return false;
}

bool Cite::adjacentALac(const Coord& pos) const {
	std::vector<std::set<Coord>> l = lacs();
	for (auto lac:l) {
		for (auto d : adjacence) {
			Coord voisin{ pos.first + d.first,pos.second + d.second };
			if (lac.count(voisin)) return true;
		}
	}
	return false;
}


int Cite::placerTuile(const TuilePtr& t, const Coord& pos) {
	if (!coupLegal(t, pos)) {
		std::cout << "\n Coup illegal";
		return 0;
	}
	int n = 0;
	for (auto& pair : t->getCoordPlateau(pos)) {
		n+=placerHex(pair.second, pair.first, t->getId());
	}
	return n;
}

bool Cite::estDansGroupeMax(const Coord& pos) const {
	return(groupeMax(plateau.at(pos).hex->getCouleur()).count(pos));
}


bool Cite::estExterieur(const Coord& c, int rayonMax) const {
	// Une coordonn�e au-del� du rayon max est ext�rieure
	return (std::ceil(std::sqrt(c.first * c.first + c.second * c.second)) > rayonMax);
}


bool Cite::estEntouree(const Coord& pos) const {
	return casesAdjacentes(pos).size() == 6;
}

//-----------------------------------------------------------------------------------------------
//                                           SET COORD
//-----------------------------------------------------------------------------------------------

std::set<Coord> Cite::casesAdjacentes(const Coord& pos) const {
	std::set<Coord> cases;
	for (auto d : Cite::adjacence) {
		Coord voisin{ pos.first + d.first,pos.second + d.second };
		if (!caseVide(voisin)) cases.insert(voisin);
	}
	return cases;
}

std::set<Coord> Cite::groupe(const Coord& pos) const {
	std::set<Coord> visites;
	std::queue<Coord> aVisiter;
	aVisiter.push(pos);
	Couleur col = plateau.find(pos)->second.hex->getCouleur();

	while (!aVisiter.empty()) {
		Coord co = aVisiter.front();
		aVisiter.pop();

		if (visites.count(co)) continue;

		auto it = plateau.find(co);
		if (it == plateau.end()) continue;

		const Case& cell = it->second;
		if (!cell.hex) continue;

		// Seulement si la case appartient au groupe
		if (cell.hex->getCouleur() != col || cell.hex->getNbEtoile() != 0) continue;

		// Maintenant oui : c�est un vrai membre du groupe
		visites.insert(co);

		for (auto d : Cite::adjacence) {
			Coord voisin{ co.first + d.first, co.second + d.second };
			if (!visites.count(voisin))
				aVisiter.push(voisin);
		}
	}

	return visites;
}

std::set<Coord> Cite::groupeMax(const Couleur& c) const {
	std::set<Coord> visites,grpMax;
	for (auto& pair : plateau) {
		if (!visites.count(pair.first) && pair.second.hex->getCouleur() == c) {
			std::set<Coord> grp=groupe(pair.first); //On cherche le groupe auquel appartient le sommet de la couleur d�sir�e
			visites.insert(grp.begin(), grp.end()); //On l'ajoute aux hexagones visit�s
			if (grp.size() > grpMax.size()) grpMax = grp;
		}
	}
	return grpMax;
}


std::set<Coord> Cite::casesAdjacentesVides() const {
	std::set<Coord> res;
	for (const auto& pair : plateau) {
		for (const auto& d : adjacence) {
			Coord coord = { pair.first.first + d.first , pair.first.second + d.second };
			if (caseVide(coord) && !res.count(coord)) res.insert(coord);
		}
	}
	return res;
}

std::vector<std::set<Coord>> Cite::lacs() const {
	std::set<Coord> nonVisites = casesAdjacentesVides();
	std::vector<std::set<Coord>> res;
	int rayon{ 0 };
	// Rayon sur le plateau principal
	for (const auto& pair : plateau) {
		int x = pair.first.first;
		int y = pair.first.second;
		int r = hex_distance(0, 0, x, y);//std::ceil(std::sqrt(x * x + y * y));
		if (r > rayon) rayon = r;
	}
	rayon += 2;//marge

	while (!nonVisites.empty()) {
		Coord start = *nonVisites.begin();
		std::queue<Coord> q;
		q.push(start);
		nonVisites.erase(start);
		std::set<Coord> comp;
		std::set<Coord> vues;
		vues.insert(start);
		bool ext = false;
		while (!q.empty() && !ext) {
			Coord cur = q.front();
			q.pop();
			if (estExterieur(cur, rayon)) {
				ext = true;
				break;
			}
			comp.insert(cur);
			for (auto& d : adjacence) {
				Coord n{ cur.first + d.first, cur.second + d.second };
				if (vues.count(n)) continue;
				vues.insert(n);
				if (estExterieur(n, rayon)) {
					ext = true;
					break;
				}
				if (!caseVide(n)) continue;
				if (nonVisites.count(n))
					nonVisites.erase(n);
				q.push(n);
			}
		}
		if (!ext) res.push_back(comp);
	}
	return res;
}

std::set<Coord> Cite::peripherie() const {
	std::set<Coord> res = casesAdjacentesVides();
	std::cout << "\n avant";
	std::vector<std::set<Coord>> l = lacs();
	std::cout << "\n apres";
	for (const auto& lac : l) {
		std::set<Coord> tmp;
		std::set_difference(res.begin(), res.end(),
			lac.begin(), lac.end(),
			std::inserter(tmp, tmp.begin()));
		res.swap(tmp);
	}
	return res;
}

std::set<Coord> Cite::peripherieHex() const {
	std::set<Coord> res;
	for (const auto& coord : peripherie()) {
		for (const auto& pos : casesAdjacentes(coord)) {
			if (!res.count(pos)) res.insert(pos);
		}
	}
	return res;
}


std::set<Coord> Cite::peripherieCouleur(const Couleur& c) const {
	std::set<Coord> periph = peripherieHex();
	std::set<Coord> res;
	for (const auto& coord : periph) {
		if (plateau.at(coord).hex->getCouleur() == c && plateau.at(coord).hex->getNbEtoile() == 0) res.insert(coord);
	}
	return res;
}

std::set<Coord> Cite::hexIsoles(const Couleur& c) const {
	std::set<Coord> res;
	for (const auto& pair : plateau) {
		if (estCouleurIsoleSaufPlace(pair.first) && pair.second.hex->getCouleur() == c && pair.second.hex->getNbEtoile() == 0) res.insert(pair.first);
	}
	return res;
}

std::set<Coord> Cite::hexEntoures(const Couleur& c) const {
	std::set<Coord> res;
	for (const auto& pair : plateau) {
		if (estEntouree(pair.first) && pair.second.hex->getCouleur() == c && pair.second.hex->getNbEtoile() == 0) res.insert(pair.first);
	}
	return res;
}

std::set<Coord> Cite::quartiers(const Couleur& c) const {
	std::set<Coord> res;
	for (const auto& pair : plateau) {
		if (pair.second.hex->getCouleur() == c && pair.second.hex->getNbEtoile() == 0) res.insert(pair.first);
	}
	return res;
}


//-----------------------------------------------------------------------------------------------
//                                           SIZE_T
//-----------------------------------------------------------------------------------------------

size_t Cite::getEtoiles(const Couleur& c) const {
	size_t n{ 0 };
	for (auto& pair : plateau) {
		if (pair.second.hex->getCouleur() == c) n += pair.second.hex->getNbEtoile();
	}
	return n;
}


int hex_distance(int x1, int y1, int x2, int y2) {
	int dq = std::abs(y1 - y2);
	int dr = std::abs(x1 - x2);
	int ds = std::abs(x2 + y2 - x1 - y1); // s1 = -x1 - y1 s2 = -x2 - y2
	if (dq > dr && dq > ds) return dq;
	else if (dr > dq && dr > ds) return dr;
	else return ds;
}