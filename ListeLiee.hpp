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
	Noeud(const T& donnee) : donnee_(donnee) {};

private:
	T donnee_;

	Noeud<T>* precedent_ = NOEUD_NUL;
	Noeud<T>* prochain_ = NOEUD_NUL;

	inline static constexpr Noeud<T>* NOEUD_NUL = nullptr;
};


template<typename T>
class Iterateur
{
	friend class ListeLiee<T>;

public:
	Iterateur(Noeud<T>* position = Noeud<T>::NOEUD_NUL) : position_(position) {};

	void avancer()
	{
		Expects(position_ != Noeud<T>::NOEUD_NUL);
		position_ = position_->prochain_;
	}

	void reculer()
	{
		Expects(position_ != Noeud<T>::NOEUD_NUL);
		position_ = position_->precedent_;
	}

	T& operator*()
	{
		Expects(position_ != Noeud<T>::NOEUD_NUL);
		return position_->donnee_;
	}

	bool operator==(const Iterateur<T>& it) const = default;
	//{
	//	return position_->donnee_ == it.position_.donnee_;
	//}

private:
	Noeud<T>* position_;
};

template<typename T>
class ListeLiee
{
	friend class Iterateur<T>;
public:
	using iterator = Iterateur<T>;

	ListeLiee() : tete_(Noeud<T>::NOEUD_NUL), queue_(Noeud<T>::NOEUD_NUL), taille_(0) {}
	~ListeLiee()
	{
		while (tete_ != queue_)
		{
			tete_ = tete_->prochain_;
			delete tete_->precedent_;
		}

		delete tete_;
	}

	bool estVide() const { return taille_ == 0; }
	unsigned size() const { return taille_; }
	iterator begin() { return { to_address(tete_) }; }
	iterator end() { return { to_address(queue_->prochain_) }; }


	void push_back(const T& item)
	{
		Noeud<T>* nouveauNoeud = new Noeud<T>(item);
		if (estVide())
			tete_ = nouveauNoeud;
		else {
			queue_->prochain_ = nouveauNoeud;
			nouveauNoeud->precedent_ = queue_;
		}
		queue_ = nouveauNoeud;
		taille_++;
	}

	iterator insert(iterator it, const T& item)
	{
		Noeud<T>* nouveauNoeud = new Noeud(item);
		Noeud<T>* prochainNoeud = it.position_;
		Noeud<T>* precedentNoeud = prochainNoeud->precedent_;

		nouveauNoeud->prochain_ = prochainNoeud;
		nouveauNoeud->precedent_ = precedentNoeud;
		prochainNoeud->precedent_ = nouveauNoeud;

		if (precedentNoeud == Noeud<T>::NOEUD_NUL)
			tete_ = nouveauNoeud;
		else
			precedentNoeud->prochain_ = nouveauNoeud;
		taille_++;

		return iterator(nouveauNoeud);
	}

	iterator erase(iterator it)
	{
		Noeud<T>* noeudIteree = it.position_;
		Noeud<T>* prochainNoeud = noeudIteree->prochain_;
		Noeud<T>* precendentNoeud = noeudIteree->precedent_;

		prochainNoeud->precedent_ = precendentNoeud;

		if (precendentNoeud == Noeud<T>::NOEUD_NUL)
			tete_ = prochainNoeud;
		else
			precendentNoeud->prochain_ = prochainNoeud;

		delete noeudIteree;
		taille_--;

		return prochainNoeud;
	}
	//TRERERE
private:
	gsl::owner<Noeud<T>*> tete_;
	Noeud<T>* queue_;

	unsigned taille_;
};
