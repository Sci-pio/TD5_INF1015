#include "Personnage.hpp"
#include "Heros.hpp"
#include "Vilain.hpp"
#include "VilainHeros.hpp"
#include "lireVectorDuFichier.hpp"
#include "lectureBinaire.hpp"
#include "ListeLiee.hpp"
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <functional>
#include <memory>
#include "cppitertools/range.hpp"
#include "bibliotheque_cours.hpp"
#include <map>
using namespace std;
using namespace iter;


ifstream ouvrirFichierBinaire(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	return fichier;
}

void testsPourCouvertureLectureBinaire()
{
	istringstream iss("\xA0\x12\xA1\x21\x43\xA2\x98\xBA\xDC\xFE");
	assert(lireUintTailleVariable(iss) == 0x12);
	assert(lireUintTailleVariable(iss) == 0x4321);
	assert(lireUintTailleVariable(iss) == 0xFEDCBA98);
}


// versReferenceAffichable permet d'avoir une référence à l'affichable, que le paramètre soit un unique_ptr ou une référence.  On aurait pu faire un template plus générique pour const/non const, et même utiliser un concept, mais ce n'est pas nécessaire pour que ça fonctionne.
// lireFichier aurait aussi pu retourner un vector de unique_ptr pour ne pas avoir besoin de versReferenceAffichable pour supporter à la fois les références et les unique_ptr.
const Affichable& versReferenceAffichable(const Affichable& p) { return p; }
template <typename T>
const Affichable& versReferenceAffichable(const unique_ptr<T>& p) { return *p; }


// Trait de separation
static const string trait =
	"═════════════════════════════════════════════════════════════════════════";

// On ne demande pas particulièrement la surcharge de << dans ce TD.
template <typename T>
void afficherAffichables(const vector<T>& affichables)
{
	static const string separateurElements = "\033[33m" + trait + "\033[0m\n";
	for (auto&& a : affichables) {
		cout << separateurElements;
		auto& x = versReferenceAffichable(a);
		x.changerCouleur(cout, 0);
		x.afficher(cout);
	}
}

// Permet d'avoir une référence non-const à un objet temporaire.
template <typename T> T& temporaireModifiable(T&& objet) { return objet; }

template <typename T>
vector<T> lireVectorDuFichier(const string& nomFichier)
{
	return lireVectorDuFichier<T>(temporaireModifiable(ouvrirFichierBinaire(nomFichier)));
}

template <typename T>
ListeLiee<T>::iterator trouverParNom(ListeLiee<T>& liste, const string& nom)
{
	//? Quand on accède à un membre dans un type qui n'est pas encore connu (ici T n'est pas connu donc ListeLiee<T> n'est pas connu non plus) lors de l'analyse syntaxique du programme, dans le cas général il ne peut pas savoir si ce membre est un type.  Pour cette raison on doit ajouter le mot typename devant pour lui dire.
	typename ListeLiee<T>::iterator fin = liste.end();
	for (typename ListeLiee<T>::iterator pos = liste.begin(); pos != fin; pos.avancer()) {
		if ((*pos).getNom() == nom)
			return pos;
	}
	return fin;
}

int main()
{
	#pragma region "Bibliothèque du cours"
	bibliotheque_cours::activerCouleursAnsi();
	#pragma endregion
	
	testsPourCouvertureLectureBinaire();
	
	static const string separateurSections = "\033[95m" + trait + "\033[0m\n";

	vector<Heros> heros = lireVectorDuFichier<Heros>("heros.bin");
	vector<Vilain> vilains = lireVectorDuFichier<Vilain>("vilains.bin");
	vector<unique_ptr<Personnage>> peronnages;

	//TODO: Transférez les héros du vecteur heros dans une ListeLiee. // Kamil: j'ai juste fait une copie dans le fond
	ListeLiee<Heros> listeHeros;
	for (int i = 0; i < heros.size(); i++) { listeHeros.push_back(heros[i]); }
	cout << "La liste de heros a une taille de " << listeHeros.size() << endl;
	
	//TODO: Créez un itérateur sur la liste liée à la position du héros Alucard.  Servez-vous de la fonction trouverParNom définie plus haut.

	Iterateur<Heros> itAlucard = trouverParNom(listeHeros, "Alucard");

	//TODO: Servez-vous de l'itérateur créé précédemment pour trouver l'héroine Aya Brea, en sachant qu'elle se trouve plus loin dans la liste, en itérant sur les éléments.

	Iterateur<Heros> itAyaBrea;
	for (Iterateur<Heros> it = itAlucard; it != listeHeros.end(); it.avancer()) {
		if ((*it).getNom() == "Aya Brea") {
			itAyaBrea = it;
			cout << "Aya Brea trouvée\n"; //kamil: affichage temporaire de test
		}
	}

	//TODO: Ajouter un hero bidon à la liste avant Aya Brea en vous servant de l'itérateur.

	Heros heroBidon("Hero bidon de test", "Jeu video", "Ennemi juré");
	listeHeros.insert(itAyaBrea, heroBidon); 

	//TODO: Assurez-vous que la taille de la liste est correcte après l'ajout.
	cout << "La liste de heros a maintenant une taille de " << listeHeros.size() << " car on a ajouté un héros bidon. " << endl;

	//TODO: Reculez votre itérateur jusqu'au héros Mario et effacez-le en utilisant l'itérateur, puis affichez le héros suivant dans la liste (devrait êter "Naked Snake/John").
	for (Iterateur<Heros> it = itAyaBrea; it != listeHeros.begin(); it.reculer() ) {
		if ((*it).getNom() == "Mario") {
			cout << "Mario trouvé\n";
			it = listeHeros.erase(it);
			(*it).afficher(cout);
		}
	}

	//TODO: Assurez-vous que la taille de la liste est correcte après le retrait.
	listeHeros.size();
	cout << "La liste de heros a maintenant une taille de " << listeHeros.size() << " car on a retiré le héros Naked Snake / John. " << endl;
	
	//TODO: Effacez le premier élément de la liste.
	Iterateur<Heros> premierElement = listeHeros.begin();
	listeHeros.erase(premierElement);
	cout << separateurSections;


	//TODO: Affichez votre liste de héros en utilisant un itérateur. La liste débute avec le héros Randi, n'a pas Mario, et le précédent de "Aya Brea" est ce que vous avez inséré. Servez-vous des methodes begin et end de la liste...
	cout << "Heros: \n";

	Iterateur<Heros> fin = listeHeros.end();
	for (Iterateur<Heros> it = listeHeros.begin(); it != fin; it.avancer()) {
		(*it).afficher(cout);
	}
	cout << separateurSections;
	//TODO: Refaite le même affichage mais en utilisant une simple boucle "for" sur intervalle.
	for (Heros& heros : listeHeros) {
		heros.afficher(cout);
	}

	//TODO: Utilisez un conteneur pour avoir les héros en ordre alphabétique (voir point 2 de l'énoncé).
	//kamil: jai verifie dans le debugger, la map est en ordre alphabetique
	map<string, Heros> uneMap;
	for (Heros& heros: listeHeros) {
		uneMap.map::insert({heros.getNom(), heros});
	}

	//TODO: Assurez-vous de n'avoir aucune ligne non couverte dans les classes pour la liste liée.  Il peut y avoir des lignes non couvertes dans les personnages...
}
