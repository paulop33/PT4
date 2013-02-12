#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
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

typedef struct sousPlateau sousPlateau;
struct sousPlateau
{
	Cellule ** matrice;
	int tailleLigne;
	int tailleCol;
};

/*Mettre toutes les cellules a 0*/
void initialiserPlateau(Plateau *p)
{	
	int i,j;

	/*allocation d'un tableau de p.taille tableaux d'entiers */
	p->matrice = malloc ( p->taille * sizeof(Cellule*) );    

	for ( i = 0 ; i < p->taille ; i ++ )
	{
		 /* allocation d'un tableau de tableau */
		 p->matrice[i] = calloc ( p->taille , sizeof(Cellule) ); 
	}

	for(i=0; i< p->taille;i++){
		for (j=0; j< p->taille;j++){
			p->matrice[i][j].val = 0;
			p->matrice[i][j].posLigne = i;
			p->matrice[i][j].posCol = j;
		}
	}

}

/*Libére la mémoire allouée pour la matrice du plateau*/
void libererMatricePlateau(Plateau *p)
{
	int i;
	for(i = 0 ; i< p->taille ; i++)
	{
		free(p->matrice[i]);
	}

	free(p->matrice);
	p->matrice = NULL;
}


/*Affiche le plateau*/
void afficherPlateau(Plateau p){
	int i,j;
	for (i=0; i<p.taille;i++){
		for (j=0; j<p.taille;j++)
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
	int maxCase = (p->taille*p->taille) ;
	int i,j;


	if(nbCelluleVivante <= maxCase )
	{
		srand(time(NULL));
			while(reste>0)
			{
				i=rand()%(p->taille);
				j=rand()%(p->taille);
				
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
	
	if(c.posLigne > 0 && p.matrice[c.posLigne-1][c.posCol].val == 1){nbV++;}//haut
	if(c.posLigne < p.taille-1 && p.matrice[c.posLigne+1][c.posCol].val == 1){nbV++;}//bas
	if(c.posCol > 0 && p.matrice[c.posLigne][c.posCol-1].val == 1){nbV++;}//gauche
	if(c.posCol < p.taille-1 && p.matrice[c.posLigne][c.posCol+1].val == 1){nbV++;}//droite

	if(c.posLigne > 0 && c.posCol > 0 && p.matrice[c.posLigne-1][c.posCol-1].val == 1){nbV++;}//haut-gauche
	if(c.posLigne > 0 && c.posCol < p.taille-1 && p.matrice[c.posLigne-1][c.posCol+1].val == 1){nbV++;}//haut-droite

	if(c.posLigne < p.taille-1 && c.posCol > 0 && p.matrice[c.posLigne+1][c.posCol-1].val == 1){nbV++;}//bas-gauche
	if(c.posLigne < p.taille-1 && c.posCol < p.taille && p.matrice[c.posLigne+1][c.posCol+1].val == 1){nbV++;}//bas-droite

	return nbV;

}

void miseAjourCellule(sousPlateau *sp,Cellule *c)
{
	if(c->val == 0 && c->nbVoisins == SEUIL_VIVANT)//si la cellule est morte et extactement 3 voisins vivants
	{
		c->val = 1;
	}
	
	else if(c->val == 1)//si elle est vivante
	{
		if(c->nbVoisins < SEUIL_MIN_MORT){ c->val=0; }// 0 ou 1 voisin vivant = meurt isolement
		
		else if(c->nbVoisins > SEUIL_MAX_MORT){ c->val=0; }// plus de 4 voisins = meurt etouffement
	}
}

int main(int argc, char *argv[])
{
	int nb;
	int i,j;


	printf("Largeur matrice : ");
	scanf("%d",&nb); 
	printf("Nombre total de cellules %d \n", nb*nb);

	Plateau p;
	p.taille = nb;
	int num_neigh[p.taille][p.taille];

	printf("Nombre de cellule vivantes : ");
	scanf("%d",&nb); 

	printf("Initilisation \n");
	initialiserPlateau(&p);
	//afficherPlateau(p);

	printf("\n\nRemplissage \n");
	remplirPlateau(&p,nb);
	afficherPlateau(p);

	
	//**************************************
	//travaille qu'effectueras un thread
	//****************************************
	sousPlateau sp;
	sp.matrice = p.matrice;// sous matrice reçoit une partie de la matrice
	sp.tailleLigne = p.taille;
	sp.tailleCol = p.taille;
	
	for(i=0; i<p.taille;i++){
		for (j=0; j<p.taille;j++){
			num_neigh[i][j]=nombreVoisinsVivants(p,p.matrice[i][j]);
			p.matrice[i][j].nbVoisins = num_neigh[i][j];
		
			
		}
	}
	
	for(i=0; i<p.taille;i++){
		for (j=0; j<p.taille;j++){
			miseAjourCellule(&sp,&sp.matrice[i][j]);
			p.matrice = sp.matrice;//mise ajour du plateau par sousPlateau
		}
	}
		
	printf("\n\nMise a jour \n");
	afficherPlateau(p);
	
	//***********************************************
	//fin travail thread
	//**************************************************
	


	printf("\n\nLiberation plateau \n");
	libererMatricePlateau(&p);



	return 0;
}
