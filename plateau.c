#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // pour le random
//ça marche pas encore tout a fait

#define LARGEUR_DAMIER 7

void initialiserPlateau(int tab[][LARGEUR_DAMIER])
{	
	int i,j;
	
	for(i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++){
			tab[i][j] = 0;
		}
	}
}


void afficherPlateau(int tab[][LARGEUR_DAMIER]){
	int i,j;
	for (i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++)
		{
			printf("%d",tab[i][j]); 
		}
		printf("\n"); 
	}
}


void remplirPlateau(int tab[][LARGEUR_DAMIER],int nbCelluleVivante)
{
	int nbRemplit = 0;
	int j,alea;
	int i = 0;
	
	
	if(nbCelluleVivante < (LARGEUR_DAMIER*LARGEUR_DAMIER))
	{
		while(i<LARGEUR_DAMIER*LARGEUR_DAMIER && nbRemplit < nbCelluleVivante)
		{
			j=0;
			while(j<LARGEUR_DAMIER*LARGEUR_DAMIER && nbRemplit < nbCelluleVivante)
			{
				printf("(%d,%d)\n",i,j);
				alea = rand()%(LARGEUR_DAMIER- 1) + 1; // 1 chance sur 3 pur alea = 1
				if(alea == 1)
				{
					tab[i][j] = 1;
					nbRemplit = nbRemplit+1;
				}
				j++;
			}
			i++;
			
			if(i == 48 && j == 48){i = 0;j =0;}//si on finit de parcourir et que nbRemplit < nbCellule on recommence
		}
	}
	
	else {printf("Désolé nombre de celulle trop grand \n");}
	
}

	
int main(int argc, char *argv[])
{
	srand(time(NULL));
	int nb=0;
	
	scanf("%d",&nb); 
	
	int tab[LARGEUR_DAMIER][LARGEUR_DAMIER];
	
	
	printf("Initilaisation \n");
	initialiserPlateau(tab);
	//afficherPlateau(tab);
	
	printf("\n\nRemplissage \n");
	remplirPlateau(tab,nb);
	afficherPlateau(tab);
	
	return 0;
}
