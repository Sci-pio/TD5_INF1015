#pragma once
#include "gsl/gsl_assert"
#include "gsl/pointers"

// Iterateur fait que acces se fait en O(1), alors que juste ListeLiee se fait en O(n)
// iterator.suivant = nouveau noeud
// iterateur.suivant.avancer() = nouveau noeud

template<typename T> class ListeLiee;
template<typename T> class Iterateur;



template<typename T>
struct Noeud
{
	friend class ListeLiee<T>;
	friend class Iterateur<T>;
public:
	//TODO: Constructeur(s).
	Noeud(const T& donnee): donnee_(donnee) {};
private:
	//TODO: Attributs d'un noeud.
	Noeud<T>* suivant_= past_end;
	Noeud<T>* precedent_ = past_end;
	T donnee_;

	inline static constexpr Noeud<T>* past_end = nullptr;
};

template<typename T>
class Iterateur
{
	friend class ListeLiee<T>;
public:
	//TODO: Constructeur(s).
	Iterateur(unique_ptr<Noeud<T>> position = Noeud<T>::past_end) {}; 
	void avancer()
	{
		Expects(position_ != nullptr);
		//TODO: Changez la position de l'itérateur pour le noeud suivant
		position_ = position_->suivant_;
	}
	void reculer()
	{
		//NOTE: On ne demande pas de supporter de reculer à partir de l'itérateur end().
		Expects(position_ != nullptr);
		//TODO: Changez la position de l'itérateur pour le noeud précédent
		position_ = position_->precedent_; //move car precedent_ est un unique_ptr

	}
	T& operator*()
	{
		Expects(position_ != Noeud<T>::past_end);
		return position_->donnee_;
	}
	//TODO: Ajouter ce qu'il manque pour que les boucles sur intervalles fonctionnent sur une ListeLiee.
	bool operator==(const Iterateur<T>& it) const = default;
private:
	Noeud<T>* position_;
};

template<typename T>
class ListeLiee
{
	friend class Iterateur<T>;
public:
	using iterator = Iterateur<T>;  // Définit un alias au type, pour que ListeLiee<T>::iterator corresponde au type de son itérateur.

	//TODO: La construction par défaut doit créer une liste vide valide.
	ListeLiee() : tete_(Noeud<T>::past_end), queue_(Noeud<T>::past_end), taille_(0){}
	~ListeLiee()
	{
		//TODO: Enlever la tête à répétition jusqu'à ce qu'il ne reste aucun élément.
		// Pour enlever la tête, 
		// 1. La tête doit devenir le suivant de la tête actuelle.
		// 2. Ne pas oublier de désallouer le noeud de l'ancienne tête (si pas fait automatiquement).
		iterator fin = ListeLiee.end();
		for (iterator pos = ListeLiee.begin(); pos != fin; pos.avancer()) {

		}
	}

	bool estVide() const  { return taille_ == 0; }
	unsigned size() const { return taille_; }
	//NOTE: to_address (C++20) permet que ce même code fonctionne que vous utilisiez des pointeurs bruts ou intelligents (ça prend le pointeur brut associé au pointeur intelligent, s'il est intelligent).
	iterator begin()  { return {to_address(tete_)}; }
	iterator end()    { return {to_address(queue_->suivant_)}; }

	// Ajoute à la fin de la liste.
	void push_back(const T& item)
	{
		Noeud<T>* nouveauNoeud = new Noeud<T>(item);
		if (estVide())
			tete_ = nouveauNoeud;
		else {
			queue_->suivant_ = nouveauNoeud;
			nouveauNoeud->precedent_ = queue_;
		}
		queue_ = nouveauNoeud;
		taille_++;
			
	}

	// Insère avant la position de l'itérateur.
	iterator insert(iterator it, const T& item)
	{
		Noeud<T>* nouveauNoeud = new Noeud(item);
		Noeud<T>* noeudSuivant = it.position_;
		Noeud<T>* noeudPrecedent = noeudSuivant->precedent_;
		nouveauNoeud->suivant_ = noeudSuivant;
		nouveauNoeud->precedent_ = noeudPrecedent;
		noeudSuivant->precedent_ = nouveauNoeud;
		if (noeudPrecedent == Noeud<T>::past_end)
			tete_ = nouveauNoeud;
		else
			tete_ = nouveauNoeud;
		taille_++;
		return iterator(nouveauNoeud);
	}

	// Enlève l'élément à la position it et retourne un itérateur vers le suivant.
	iterator erase(iterator it)
	{
		//TODO: Enlever l'élément à la position de l'itérateur.
		//  1. Le pointeur vers le Noeud à effacer est celui dans l'itérateur.
		//  2. Modifiez l'attribut suivant_ du noeud précédent pour que celui-ci
		//     pointe vers le noeud suivant la position de l'itérateur (voir 1.).
		//  3. Modifiez l'attribut precedent_ du noeud suivant la position de
		//     l'itérateur pour que celui-ci pointe vers le noeud précédent
		//     de la position de l'itérateur (voir 1.).
		//  4. Désallouez le Noeud à effacer (voir 1.).
		//  5. Décrémentez la taille de la liste
		//  6. Retournez un itérateur vers le suivant du Noeud effacé.
		//TODO: On veut supporter d'enlever le premier élément de la liste,
		//  donc en 2. il se peut qu'il n'y ait pas de précédent et alors c'est
		//  la tête de liste qu'il faut ajuster.
		//NOTE: On ne demande pas de supporter d'effacer le dernier élément (c'est similaire au cas pour enlever le premier).
	}

private:
	gsl::owner<Noeud<T>*> tete_;  //NOTE: Vous pouvez changer le type si vous voulez. // 

	Noeud<T>* queue_;
	unsigned taille_;
};
