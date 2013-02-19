#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "CONSTANTES.h"


static int nbThreadLance=0;  /* Le nombre de thread lancee */ 
static pthread_mutex_t m1; /*  Mutex pour bloquer lors de la numérotation des threads */
/* Mutex pour bloquer attendre que le calcul des voisins soit effectué par tous les threads 
avant de commencer à traiter la mise à jour des cellules */
static pthread_mutex_t m2; 


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

static Plateau p;

/*Mettre toutes les cellules a 0*/
void initialiserPlateau(Plateau *p)
{
	int i,j;

	/*allocation d'un tableau de p.taille tableaux d'entiers */
	p->matrice = malloc ( (p->taille+2) * sizeof(Cellule*) );

	for ( i = 0 ; i < p->taille+2 ; i ++ )
	{
		/* allocation d'un tableau de tableau */
		p->matrice[i] = calloc ( (p->taille+2) , sizeof(Cellule) );
	}

	for(i=0; i< p->taille+2;i++){
		for (j=0; j< p->taille+2;j++){
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
	for(i = 0 ; i< p->taille+2 ; i++)
	{
		free(p->matrice[i]);
	}

	free(p->matrice);
	p->matrice = NULL;
}


/*Affiche le plateau*/
void afficherPlateau(Plateau p){
	int i,j;
	for (i=0; i<p.taille+2;i++){
		for (j=0; j<p.taille+2;j++)
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
			//[a;b] (b-a)+a <=> taille+1 -1
			i=rand()%(p->taille)+1;
			j=rand()%(p->taille)+1;

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
	if(c.posLigne < p.taille && p.matrice[c.posLigne+1][c.posCol].val == 1){nbV++;}//bas
	if(c.posCol > 0 && p.matrice[c.posLigne][c.posCol-1].val == 1){nbV++;}//gauche
	if(c.posCol < p.taille && p.matrice[c.posLigne][c.posCol+1].val == 1){nbV++;}//droite
	if(c.posLigne > 0 && c.posCol > 0 && p.matrice[c.posLigne-1][c.posCol-1].val == 1){nbV++;}//haut-gauche
	if(c.posLigne > 0 && c.posCol < p.taille && p.matrice[c.posLigne-1][c.posCol+1].val == 1){nbV++;}//haut-droite
	if(c.posLigne < p.taille && c.posCol > 0 && p.matrice[c.posLigne+1][c.posCol-1].val == 1){nbV++;}//bas-gauche
	if(c.posLigne < p.taille && c.posCol < p.taille && p.matrice[c.posLigne+1][c.posCol+1].val == 1){nbV++;}//bas-droite

	return nbV;
}

void miseAjourCellule(Cellule *c)
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
void copieDesBords(Plateau p){
	int i=0;
	p.matrice[0][0].val=p.matrice[p.taille][p.taille].val;
	p.matrice[0][p.taille+1].val=p.matrice[p.taille][1].val;
	p.matrice[p.taille+1][0].val=p.matrice[1][p.taille].val;
	p.matrice[p.taille+1][p.taille+1].val=p.matrice[1][1].val;
	for(i=1;i<p.taille+1;i++){
		p.matrice[0][i].val=p.matrice[p.taille][i].val;
		p.matrice[p.taille+1][i].val=p.matrice[1][i].val;
		p.matrice[i][0].val=p.matrice[i][p.taille].val;
		p.matrice[i][p.taille+1].val=p.matrice[i][1].val;
	}
}
/* Fonction executée par un thread */ 
void * thread(){
	int debutLigneTraitement=0; // ligne à partir de laquelle le thread va commencer son traitement
	int finTraitement; // ligne à laquelle le thread le thread arrete son traitement (cette ligne n'est pas compris)
	int numThread; // numero du thread
	
	// On bloque avec un mutex afin qu'un thread n'est pas le num d'un autre
	pthread_mutex_lock(&m1); 
	numThread=nbThreadLance;
	nbThreadLance++;
	pthread_mutex_unlock(&m1);
	
	debutLigneTraitement=numThread*(LARGEUR_PLATEAU/NB_THREADS);
	finTraitement=debutLigneTraitement+1+(LARGEUR_PLATEAU/NB_THREADS);
	
	int numTour=0; /** Nombre de tour */
	int i,j;
	while (numTour<NB_TOUR){
		numTour++;
		if (numThread==0) //si c'est le premier thread => nouvelle itération
			copieDesBords(p);
			
	    for(i = debutLigneTraitement+1; i< finTraitement ; i++){
			for (j=1; j<p.taille+1;j++){    
		        p.matrice[i][j].nbVoisins = nombreVoisinsVivants(p,p.matrice[i][j]);
			}
	    }
	    // Chaque thread doit attendre que les autres aient terminé le calcul des voisins
	    
	    for(i=debutLigneTraitement+1; i< finTraitement ;i++){
          	for (j=1; j<p.taille+1;j++){
            	miseAjourCellule(&p.matrice[i][j]);
			}
	    }
	    //~ afficherPlateau(p);
	    printf("%d\n",numTour);
	    //~ usleep(1000);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int nb,i;
	
	//on déclare les threads
	pthread_t th[NB_THREADS];

	/** Initialisation des mutex */
	pthread_mutex_init(&m1,NULL);
	pthread_mutex_init(&m2,NULL);
	
	//commentaire
	printf("Largeur matrice : ");
	//scanf("%d",&nb);
	nb=LARGEUR_PLATEAU;
	printf("Nombre total de cellules %d \n", nb*nb);

	p.taille = nb;
	printf("Nombre de cellule vivantes : ");
	//scanf("%d",&nb);
    nb=NB_CELLULE_VIVANTE_DEPART;
    
	printf("Initilisation \n");
	initialiserPlateau(&p);
	//afficherPlateau(p);

	printf("\n\nRemplissage \n");
	remplirPlateau(&p,nb);
	//afficherPlateau(p);


	//**************************************
	// Lancement des Threads
	//****************************************
	int ret; // valeur de retour lors de la création d'un thread
	for(i=0;i<NB_THREADS;i++){
		ret = pthread_create(&(th[i]), NULL, thread,NULL);
		if(ret) {
			fprintf(stderr, "Impossible de créer le thread %d\n", i);
			return 1;
		}
	}

	void *ret_val;
	for(i=0;i<NB_THREADS;i++) 
	{
		int ret = pthread_join(th[i], &ret_val);
		if(ret) {
			fprintf(stderr, "Erreur lors de l'attente du thread %d\n", i);
			return 1;
		}
	}

	//***********************************************
	//fin travail thread
	//**************************************************

	printf("\n\nMise a jour \n");
	//~ afficherPlateau(p);

	/** Destruction des mutex */
	pthread_mutex_destroy(&m1);
	pthread_mutex_destroy(&m2);
	
	printf("\n\nLiberation plateau \n");
	libererMatricePlateau(&p);

	return 0;
}
