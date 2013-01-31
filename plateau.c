#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define LARGEUR_DAMIER 7


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
	Cellule matrice[LARGEUR_DAMIER][LARGEUR_DAMIER];
};

/*Mettre toutes les cellules a 0*/
void initialiserPlateau(Plateau *p)
{	
	int i,j;
	
	for(i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++){
			p->matrice[i][j].val = 0;
			p->matrice[i][j].posLigne = j;
			p->matrice[i][j].posCol = i;//Toutes les celulles du plateau a 0
		}
	}
}

/*Affiche le plateau*/
void afficherPlateau(Plateau p){
	int i,j;
	for (i=0; i<LARGEUR_DAMIER;i++){
		for (j=0; j<LARGEUR_DAMIER;j++)
		{
			printf("%d",p.matrice[i][j].val); 
		}
		printf("\n"); 
	}
	printf("\n"); 
}

/*Crée des cellules vivantes selon le nombre rentré*/
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
				if(p->matrice[i][j].val!=1)//si la valeur de la cellule n'est pas vivante
				{
					p->matrice[i][j].val=1;
					reste--;
				}
			}
		}
	
	else {printf("Désolé nombre de celulle trop grand \n");}
	
}

int nombreVoisinsVivants(Plateau p, Cellule c)
{
	int nbV = 0;
	
	if(c.posLigne > 1 && c.posLigne < LARGEUR_DAMIER-1 && c.posCol > 1 && c.posCol < LARGEUR_DAMIER-1)//Cellules vers le centre
	{
		if(p.matrice[c.posLigne-1][c.posCol].val == 1){nbV++;}//haut
		if(p.matrice[c.posLigne+1][c.posCol].val == 1){nbV++;}//bas
		if(p.matrice[c.posLigne][c.posCol-1].val == 1){nbV++;}//gauche
		if(p.matrice[c.posLigne][c.posCol+1].val == 1){nbV++;}//droite
		
		if(p.matrice[c.posLigne-1][c.posCol-1].val == 1){nbV++;}//haut-gauche
		if(p.matrice[c.posLigne-1][c.posCol+1].val == 1){nbV++;}//haut-droite
		
		if(p.matrice[c.posLigne+1][c.posCol-1].val == 1){nbV++;}//bas-gauche
		if(p.matrice[c.posLigne+1][c.posCol+1].val == 1){nbV++;}//bas-droite
	
	}
	
	//else if(){}
	printf("Cellule (%d,%d) %d voisins \n",c.posLigne,c.posCol,nbV);
	return nbV;
	
}


int main(int argc, char *argv[])
{
	int nb;
	int i,j;
	
	printf("Nombre de cellule vivantes : ");
	scanf("%d",&nb); 
	
	Plateau p;
	
	
	printf("Initilisation \n");
	initialiserPlateau(&p);
	afficherPlateau(p);
	
	printf("\n\nRemplissage \n");
	remplirPlateau(&p,nb);
	afficherPlateau(p);
	
	
	for(i=1; i<LARGEUR_DAMIER-1;i++){
		for (j=0; j<LARGEUR_DAMIER-1;j++){
			nombreVoisinsVivants(p,p.matrice[i][j]);
		}
	}
	
	
	
	return 0;
}
