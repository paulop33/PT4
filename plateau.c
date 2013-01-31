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
		/*while(i<maxCase && reste >0)
		{
			j=0;
			while(j<maxCase&& reste >0)
			{
				alea = rand()%(3 - 1) + 1; // 1 chance sur 3 pur alea = 1
				if(alea==1 && tab[i][j]==0)
				{
					tab[i][j] = 1;
					reste--;
				}
				j++;
			}
			i++;
			
			if(i == 48 && j == 48 && reste > 0){i = 0;j =0;}//si on finit de parcourir et que nbRemplit < nbCellule on recommence
		}*/

			while(reste>0)
			{
				i=rand()%(maxCase-1);
				j=rand()%(maxCase-1);
				if(p->matrice[i][j]!=1)
				{
					p->matrice[i][j]=1;
					//printf("%d",p->matrice[i][j]);
					reste--;
				}
			}
			//en sortant de la boucle while certaines case qui avait été passer à 1 revienne à 0

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
