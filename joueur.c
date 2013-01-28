#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char* argv[])
{
 //Init rand
	int max=3.0;
	srand(time(NULL)+getpid());
	int alea= (int)(rand()%3);
	exit(alea);
}