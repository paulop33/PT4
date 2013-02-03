#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//#define LARGEUR_DAMIER 7


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
			p->matrice[i][j].posLigne = j;
			p->matrice[i][j].posCol = i;//Toutes les celulles du plateau a 0
		}
	}
		
}

/*Libére la mémoire allouée pour la matrice du plateau*/
void libererMatricePlateau(Plateau *p)
{
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
				i=rand()%(p->taille -1);
				j=rand()%(p->taille -1);
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
	
	if(c.posLigne > 1 && c.posLigne < p.taille-1 && c.posCol > 1 && c.posCol < p.taille-1)//Cellules vers le centre
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
	
	printf("Largeur matrice : ");
	scanf("%d",&nb); 
	printf("Nombre total de cellules %d \n", nb*nb);
	
	Plateau p;
	p.taille = nb;
	
	printf("Nombre de cellule vivantes : ");
	scanf("%d",&nb); 
	
	printf("Initilisation \n");
	initialiserPlateau(&p);
	afficherPlateau(p);
	
	printf("\n\nRemplissage \n");
	remplirPlateau(&p,nb);
	afficherPlateau(p);
	
	/*for(i=1; i<p.taille-1;i++){
		for (j=0; j<p.taille-1;j++){
			nombreVoisinsVivants(p,p.matrice[i][j]);
		}
	}*/
	
	
	printf("\n\nLiberation plateau \n");
	libererMatricePlateau(&p);
	
	
	
	
	
	
	return 0;
}
