#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



int main( int argc, char* argv[])
{
 //Init rand

	float max=3.0;
	int alea= 0;
	int r;
	int i;
	pid_t pid;
	int status;
	int nb_joueurs;


	//Arguments
	if(argc==2)
	{
		nb_joueurs=atoi(argv[1]);
	}
	else
	{
		fprintf(stdout, "Erreur paramètres\n");
		exit(0);

	}

	int tableauJeu[nb_joueurs]; //Tableau pour stockage jeu
	int tableauPts[nb_joueurs]; //Tableau pour stockage points
	int tableauPro[nb_joueurs]; //Tableau pour stockage points

	for(i=1; i<=nb_joueurs; i++)
	{
		tableauPts[i]=0;
		tableauJeu[i]=0;
		tableauPro[i]=0;
	}

	for(i=1; i<=nb_joueurs; i++)
	{
		if((r=fork())<0)
		{
			fprintf(stderr, "Erreur fatale : fork()\n");
			exit(EXIT_FAILURE);
		}

		if(r == 0)
		{ //Fils
			srand(time(NULL)+getpid());
			alea= (int)(max*rand()/RAND_MAX);
			exit(alea);
		}
	}


  	// père
  	for (i=1; i<=nb_joueurs; i++)
    {
      pid = wait(&status);

      printf("Processus %d joue (%d) ", pid, WEXITSTATUS(status));

      switch(WEXITSTATUS(status))
      {
      	case 0:
      		printf("PAPIER");
      	break;
      	case 1:
      		printf("PIERRE");
      	break;
      	default:
			printf("CISEAU");
      	break;
      }
      printf("\n");

      tableauJeu[i]= WEXITSTATUS(status);
      tableauPro[i]= pid;
 
    }

    int j;
    printf("\n\n Points des Processus \n");
   
   	for(i=1; i<=nb_joueurs; i++)
   	{
   		for(j=1 && j!=i;  j<=nb_joueurs; j++)
   		{
   			if(tableauJeu[i] != tableauJeu[j])
   			{
   				if(    (tableauJeu[i]==0 && tableauJeu[j]==1)
   					|| (tableauJeu[i]==1 && tableauJeu[j]==2)
   					|| (tableauJeu[i]==2 && tableauJeu[j]==0))	

   						tableauPts[i]++;
   			}
   		}
   	}

   	//Affichage score
   	for(i=1; i<=nb_joueurs; i++)
   		printf("Processus: %d Point: %d  \n", tableauPro[i], tableauPts[i]);
  
}