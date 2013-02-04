#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h> // pour le random


#define LARGEUR_DAMIER 7
#define DELAI 1

signal(SIGALRM, onalrm); 
alarm(DELAI); 

void initialiserTab(int tab[][LARGEUR_DAMIER]){
	int i,j;
	for (i=0; i<LARGEUR_DAMIER;i++)
		for (j=0; j<LARGEUR_DAMIER;j++)
			tab[i][j]=0;
	tab[5][2]=1;
}

void afficherGrille(int tab[][LARGEUR_DAMIER]){
	int i,j;
	for (i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++)
		{
			printf("%d",tab[i][j]); 
		}
		printf("\n"); 
	}
}

int main(int argc, char**argv)
{
	int tab[LARGEUR_DAMIER][LARGEUR_DAMIER];
	//On dimensionne le tableau dynamique grâce aux valeurs saisies par l'utilisateur
    initialiserTab(tab)
	while (TRUE) {
	
	}
	afficherGrille(tab);
	return 0;
}

void onalrm () 
{
    printf('zqrjdk'); 
    signal(SIGALRM,onalrm);
    alarm(DELAI);
}