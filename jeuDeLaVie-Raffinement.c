#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>
#include "CONSTANTES.h"
static int NB_THREADS=4; /* Nombre de threads */
static int NB_TOUR=5;


/* Les semaphores pour un blocage local (juste autour de notre thread)*/
static sem_t *sem_apres_copie, *sem_apres_calcul_voisin;

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

/**
* Creation des cellules 
*/
void initialiserPlateau(Plateau *p)
{
	int i,j;

	/*allocation d'un tableau de p.taille tableaux d'entiers */
	p->matrice = malloc ( (p->taille+2) * sizeof(Cellule*) );

	for(i=0; i< p->taille+2;i++){
		/* allocation d'un tableau de tableau */
		p->matrice[i] = calloc ( (p->taille+2) , sizeof(Cellule) );
		for (j=0; j< p->taille+2;j++){
			p->matrice[i][j].val = 0;
			p->matrice[i][j].posLigne = i;
			p->matrice[i][j].posCol = j;
		}
	}
}

/**
* Libére la mémoire allouée pour la matrice du plateau
*/
void libererMatricePlateau(Plateau *p){
	int i;
	for(i = 0 ; i< p->taille+2 ; i++){
		free(p->matrice[i]);
	}
	free(p->matrice);
	p->matrice = NULL;
}

/**
* Affiche le plateau dans le terminal
*/
void afficherPlateau(Plateau p){
	int i,j;
	//system("clear");
	for (i=0; i<p.taille+2;i++){
		if(i== p.taille+1) printf("_____________\n");
		if(i== 1) printf("____________\n");

		for (j=0; j<p.taille+2;j++){
			if(j== p.taille+1) 
				printf(" |");
			printf("%d",p.matrice[i][j].val);
			
			if(j== 0)
				printf(" |");
		}
		printf("\n");
	}
	printf("\n");
}

/**
* Crée des cellules vivantes selon le nombre rentré
*/
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
	else 
		printf("Désolé nombre de celulle trop grand \n");
}

/**
* return nombre de voisin de la cellule passee en parametre
*/
int nombreVoisinsVivants(Plateau p, Cellule c)
{
	int nbV = 0;

	if(c.posLigne > 0 && p.matrice[c.posLigne-1][c.posCol].val == 1) nbV++;//haut
	if(c.posLigne < p.taille && p.matrice[c.posLigne+1][c.posCol].val == 1) nbV++;//bas
	if(c.posCol > 0 && p.matrice[c.posLigne][c.posCol-1].val == 1) nbV++; //gauche
	if(c.posCol < p.taille && p.matrice[c.posLigne][c.posCol+1].val == 1) nbV++;//droite
	if(c.posLigne > 0 && c.posCol > 0 && p.matrice[c.posLigne-1][c.posCol-1].val == 1) nbV++;//haut-gauche
	if(c.posLigne > 0 && c.posCol < p.taille && p.matrice[c.posLigne-1][c.posCol+1].val == 1) nbV++;//haut-droite
	if(c.posLigne < p.taille && c.posCol > 0 && p.matrice[c.posLigne+1][c.posCol-1].val == 1) nbV++;//bas-gauche
	if(c.posLigne < p.taille && c.posCol < p.taille && p.matrice[c.posLigne+1][c.posCol+1].val == 1) nbV++;//bas-droite

	return nbV;
}

/**
* Met a jour la cellule passee en parametre
*/
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

/**
* Copie les bords ainsi que les coins a l'oppose
*/
void copieDesBords(Plateau p,int debutLigneTraitement, int finTraitement, int numThread){
	int i=0;
	if (numThread==0){
		p.matrice[p.taille+1][0].val=p.matrice[1][p.taille].val; //coin bas gauche
		p.matrice[p.taille+1][p.taille+1].val=p.matrice[1][1].val; //coin bas droite
		for(i=1;i<p.taille+1;i++){
			p.matrice[0][i].val=p.matrice[p.taille][i].val; //modification dessus
		}
	}
	else if (numThread==NB_THREADS-1){
		p.matrice[0][0].val=p.matrice[p.taille][p.taille].val;  //coin haut gauche
		p.matrice[0][p.taille+1].val=p.matrice[p.taille][1].val; //coin haut droit
		for(i=1;i<p.taille+1;i++)
			p.matrice[p.taille+1][i].val=p.matrice[1][i].val; //desous
	}
}

/**
* Fonction executée par un thread 
*/ 
void * thread(void *numeroThread){
	// numero du thread
	int numThread=(int)numeroThread; 
	//variable pour la stagnation
	int nb_cellule_bord_vivante=0;
	int nb_cellule_inter_change=0;
	
	// ligne à partir de laquelle le thread va commencer son traitement
	int debutLigneTraitement= debutLigneTraitement= numThread *(LARGEUR_PLATEAU / NB_THREADS)+1;
	// ligne à laquelle le thread arrete son traitement (cette ligne n'est pas compris)
	int finTraitement= debutLigneTraitement + (LARGEUR_PLATEAU / NB_THREADS);
	if(numThread == NB_THREADS - 1)//si dernier thread
	{
		finTraitement+= LARGEUR_PLATEAU % NB_THREADS;
	}
	
	int precedent= (numThread==0) ? NB_THREADS-1 : numThread - 1;
	int suivant= (numThread==NB_THREADS-1) ? 0 : numThread + 1;
	int numTour=0; /** Nombre de tour *4 */
	int i,j;
	for (numTour = 0; i < NB_TOUR; i++)
	 {
		copieDesBords(p,debutLigneTraitement,finTraitement,numThread);
		//afficherPlateau(p);

	    //Copie des bords "extérieur" qui ont une insidence sur le thread courant
	    p.matrice[debutLigneTraitement][0].val = p.matrice[debutLigneTraitement][p.taille].val;     
	    p.matrice[debutLigneTraitement][p.taille+1].val = p.matrice[debutLigneTraitement][1].val;
	    p.matrice[finTraitement-1][0].val = p.matrice[finTraitement-1][p.taille].val;     
	    p.matrice[finTraitement-1][p.taille+1].val = p.matrice[finTraitement-1][1].val;

		// On previent les voisins que les copies des bords indispensables pour leurs calculs sont terminees
	    sem_post(&sem_apres_copie[suivant]);
	    sem_post(&sem_apres_copie[precedent]);

		for (i = debutLigneTraitement+1; i <finTraitement-1 ; i++)
		{ // on copie les bords internes (qui n'ont pas de conséquence sur les calculs des autres threads)
			p.matrice[i][0].val=p.matrice[i][p.taille].val;	 //gauche
			p.matrice[i][p.taille+1].val=p.matrice[i][1].val; //droite
		}
	    sem_wait(&sem_apres_copie[numThread]);
	    sem_wait(&sem_apres_copie[numThread]);

		// Calcul du nombre de voisins pour cellules extérieures
	    for(i = 1; i< p.taille ; i++){
		    p.matrice[debutLigneTraitement][i].nbVoisins = nombreVoisinsVivants(p,p.matrice[i][j]);
		    p.matrice[finTraitement-1][i].nbVoisins = nombreVoisinsVivants(p,p.matrice[i][j]);
	    }
		// On prévient les voisins que l'on a fini de traiter les bords
	    sem_post(&sem_apres_calcul_voisin[suivant]);
	    sem_post(&sem_apres_calcul_voisin[precedent]);
		// Calcul du nombre de voisins pour cellules intérieurs
	    for(i = debutLigneTraitement; i< finTraitement ; i++){
			for (j=1; j<p.taille+1;j++){    
		        p.matrice[i][j].nbVoisins = nombreVoisinsVivants(p,p.matrice[i][j]);
			}
	    }
	    sem_wait(&sem_apres_calcul_voisin[numThread]);
	    sem_wait(&sem_apres_calcul_voisin[numThread]);
	    
	    //application MAJ des cellules
	    for(i=debutLigneTraitement; i< finTraitement ;i++){
          	for (j=1; j<p.taille+1;j++){
            	miseAjourCellule(&p.matrice[i][j]);
			}
	    }
	    
	}
	pthread_exit(0);
	return NULL;
}

int main(int argc, char *argv[])
{
	int i;
	if (argc == 3){
  		NB_TOUR = atoi(argv[1]);
  		NB_THREADS = atoi(argv[2]);
	}
	//on déclare les threads
	pthread_t th[NB_THREADS];

	// Initialisation des semaphores
 	sem_apres_copie = malloc(NB_THREADS * sizeof(sem_t)); 
	sem_apres_calcul_voisin = malloc(NB_THREADS * sizeof(sem_t));
	for(i = 0; i < NB_THREADS; i++){
		sem_init(&sem_apres_copie[i], 0, 0);
	    sem_init(&sem_apres_calcul_voisin[i], 0, 0); 
	}

	p.taille = LARGEUR_PLATEAU;
    
	//Initilisation du plateau
	initialiserPlateau(&p);
	//afficherPlateau(p);

	//Remplissage
	remplirPlateau(&p,NB_CELLULE_VIVANTE_DEPART);
	//afficherPlateau(p);

	//**************************************
	// Lancement des Threads
	//**************************************
	int ret; // valeur de retour lors de la création d'un thread
	for(i=0;i<NB_THREADS;i++){
		ret = pthread_create(&(th[i]), NULL, thread,(void *)(int)i);
		if(ret) {
			fprintf(stderr, "Impossible de créer le thread %d\n", i);
			return 1;
		}
	}

	//**************************************
	// Attente des Threads
	//**************************************
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
	//***********************************************

	printf("\n\nLiberation plateau \n");
	libererMatricePlateau(&p);

	return 0;
}