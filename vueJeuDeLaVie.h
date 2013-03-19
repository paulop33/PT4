#ifndef vueJeuDeLaVie
#define vueJeuDeLaVie
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "CONSTANTES.h"

typedef struct Cellule Cellule;
struct Cellule
{
	int val;
	int nbVoisins;
	int posLigne;
	int posCol;
};


typedef struct Plateau Plateau;
struct Plateau
{
	Cellule ** matrice;
	int taille;

};

	 void afficherVue(Plateau p);
	
#endif
