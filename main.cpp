// Auteurs: Leonard Pouliot (2150965) et Kamil Maarite (2152653)
// Date: 20 novembre 2022
// Cours: INF1015
// Nom de la classe: main.cpp

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


const Affichable& versReferenceAffichable(const Affichable& p) { return p; }
template <typename T>
const Affichable& versReferenceAffichable(const unique_ptr<T>& p) { return *p; }


static const string trait =
	"═════════════════════════════════════════════════════════════════════════";


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

template <typename T> T& temporaireModifiable(T&& objet) { return objet; }

template <typename T>
vector<T> lireVectorDuFichier(const string& nomFichier)
{
	return lireVectorDuFichier<T>(temporaireModifiable(ouvrirFichierBinaire(nomFichier)));
}

template <typename T>
ListeLiee<T>::iterator trouverParNom(ListeLiee<T>& liste, const string& nom)
{
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

	//NOTE: Les consignes n'indiquaient pas que nos affichages devaient être faits en couleur, donc nous ne l'avons pas fait. 

	//Transférez les héros du vecteur heros dans une ListeLiee. 
	ListeLiee<Heros> listeHeros;
	for (int i = 0; i < heros.size(); i++) { listeHeros.push_back(heros[i]); }
	cout << "La liste de heros a une taille de " << listeHeros.size() << endl;
	

	//Créez un itérateur sur la liste liée à la position du héros Alucard.
	Iterateur<Heros> itAlucard = trouverParNom(listeHeros, "Alucard");


	//Servez-vous de l'itérateur créé précédemment pour trouver l'héroine Aya Brea.
	Iterateur<Heros> itAyaBrea;
	for (Iterateur<Heros> it = itAlucard; it != listeHeros.end(); it.avancer()) {
		if ((*it).getNom() == "Aya Brea") {
			itAyaBrea = it;
			cout << "Aya Brea trouvée\n"; 
		}
	}


	//Ajouter un hero bidon à la liste avant Aya Brea en vous servant de l'itérateur.
	Heros heroBidon("Hero bidon de test", "Jeu video", "Ennemi juré");
	listeHeros.insert(itAyaBrea, heroBidon); 


	//Assurez-vous que la taille de la liste est correcte après l'ajout.
	cout << "La liste de heros a maintenant une taille de " << listeHeros.size() << " car on a ajouté un héros bidon. " << endl;


	//Reculez votre itérateur jusqu'au héros Mario et effacez-le en utilisant l'itérateur, puis affichez le héros suivant dans la liste.
	for (Iterateur<Heros> it = itAyaBrea; it != listeHeros.begin(); it.reculer() ) {
		if ((*it).getNom() == "Mario") {
			cout << "Mario trouvé\n";
			it = listeHeros.erase(it);
			(*it).afficher(cout);
		}
	}


	//Assurez-vous que la taille de la liste est correcte après le retrait.
	listeHeros.size();
	cout << "La liste de heros a maintenant une taille de " << listeHeros.size() << " car on a retiré le héros Naked Snake / John. " << endl;
	

	//Effacez le premier élément de la liste.
	Iterateur<Heros> premierElement = listeHeros.begin();
	listeHeros.erase(premierElement);
	cout << separateurSections;


	//Affichez votre liste de héros en utilisant un itérateur. 
	cout << "Heros: \n";

	Iterateur<Heros> fin = listeHeros.end();
	for (Iterateur<Heros> it = listeHeros.begin(); it != fin; it.avancer()) {
		(*it).afficher(cout);
	}
	cout << separateurSections;


	//Refaite le même affichage mais en utilisant une simple boucle "for" sur intervalle.
	cout << "Heros: \n";

	for (Heros& heros1 : listeHeros) {
		heros1.afficher(cout);
	}
	cout << separateurSections;


	//Utilisez un conteneur pour avoir les héros en ordre alphabétique.
	map<string, Heros> uneMap;
	for (Heros& heros2: listeHeros) {
		uneMap.map::insert({heros2.getNom(), heros2});
	}

	Heros rockman = uneMap["Rockman/Mega Man"];
	rockman.afficher(cout);

	// Question 2.2
	/* La complexité de cette recherche est O(log(n)), où n est la taille de la map. En effet, comme les éléments dans uneMap sont triés
	en ordre d'alphabétique de leur key (le nom du héros qui est stockée dans la deuxième valeur de la pair), le nombre d'opérations 
	élémentaires en O(1) à affectuer pour trouver un élément est en moyenne logarithmique par rapport à la taille de la map. L'algorithme
	se fait ainsi: on regarde au milieu du tableau, et selon si la valeur observée est plus grande ou plus petite que celle qu'on recherche, 
	l'algorithme décide en O(1) (simple comparaison) quelle moitié considérer ensuite. Tant qu'il y a plus d'un élément dans le tableau à considérer
	et que la key voulue n'a pas été trouvée, ce processus continue. Le nombre de comparaisons à effectuer va donc de 1 (si le premier élément 
	considéré est la bon) au log(base 2) de n, ce qui est la même chose que le log (base 10) de n / log (base 2) n, donc on peut écrire seulement
	log(n). En moyenne, la complexité est en fait est un multiple inférieur à 1 * log(n), car il est possible de trouver la key avant de se rendre
	jusqu'au bout de l'arbre, donc la complexité reste O(log(n)).
	*/

	// Question 2.3
	/* La liste liée effectue ses recherches en O(n) où n est la taille de la liste, car dépendemment de la position relative de l'élément 
	cherché par rapport à l'itérateur, le nombre d'opérations élémentaires à effectuer (faire avancer ou reculer l'itérateur et comparer le
	nom) est proportionnel à n. Notons que dans ce tp les consignes nous indiquaient si l'élément recherché était avant ou après la position
	de notre itérateur, mais en pratique cette information n'est pas connue donc il faudrait commencer au début ou à la fin de la liste pour
	une recherche - pour trouver Aya Brea, il aurait fallu avancer l'itérateur et comparer le nom 9 fois. Pour de grands ensembles de données,
	le O(log(n)) de la map battrait le O(n) de la liste liée, et même s'il n'y a que 9 héros ici le logarithme l'emporte pareil en moyenne, 
	car log(base 2) de n est plus petit que n pour tout n. Dans certains cas, par exemple si on cherche le deuxième élément de la liste liée
	et qu'on commence au début, la liste liée sera plus rapide, mais en moyenne ce ne sera pas le cas.
	*/
}
