#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define LARGEUR_DAMIER 7

typedef struct Plateau Plateau;
struct Plateau
{
	int matrice[LARGEUR_DAMIER][LARGEUR_DAMIER];
};

void initialiserPlateau(Plateau *p)
{	
	int i,j;
	
	for(i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++){
			p->matrice[i][j] = 0;
		}
	}
}


void afficherPlateau(Plateau p){
	int i,j;
	for (i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++)
		{
			printf("%d",p.matrice[i][j]); 
		}
		printf("\n"); 
	}
	printf("\n"); 
}

void remplirPlateau(Plateau *p,int nbCelluleVivante)
{
	int reste = nbCelluleVivante;
	int maxCase = LARGEUR_DAMIER*LARGEUR_DAMIER;
	int i,j;
	
	
	if(nbCelluleVivante <= maxCase )
	{
		srand(time(NULL));

			while(reste>0)
			{
				i=rand()%(LARGEUR_DAMIER-1);
				j=rand()%(LARGEUR_DAMIER-1);
				if(p->matrice[i][j]!=1)
				{
					p->matrice[i][j]=1;
					reste--;
				}
			}
		}
	
	else {printf("Désolé nombre de celulle trop grand \n");}
	
}
	
int main(int argc, char *argv[])
{
	int nb;
	
	printf("Nombre de cellule vivantes : ");
	scanf("%d",&nb); 
	
	Plateau p;
	
	
	printf("Initilaisation \n");
	initialiserPlateau(&p);
	afficherPlateau(p);
	
	printf("\n\nRemplissage \n");
	remplirPlateau(&p,nb);
	afficherPlateau(p);
	
	return 0;
}
