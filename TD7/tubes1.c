/* BELDJILALI IENAC15_SITA  TD7 */
/* transfert  à travers des tubes */
/* gcc -Wall -W -lpthread -o tubes1 tubes1.c */

#define _REENTRANT
#include <fcntl.h> /* pour read et open*/
#include <stdio.h> /* pour perror et printf */
#include <unistd.h>  /* pour fork() */
#include <stdlib.h>  /* pour exit() */
#include <sys/wait.h> /* pour wait */
#include <signal.h>  /* pour sleep */
//#include <string.h>

#define FALSE 0
#define TRUE 1

int main(int argc,  char *argv[]){
  int pid1=0, pid2=0;
  int status;  /* pour code retour wait()*/
  int i, fd, nb;
  char buf[512];
  int ecart = 'a' - 'A';

  int tube[2];
  
  
     if (argc==2)
	{
		if ((fd=open(argv[1], O_RDONLY))==-1)
		{
    			perror("Ouverture");
    			exit(1);
  		}
	}
	else
	{
                printf("Erreur ! Veuillez préciser un nom de fichier à lire ici fic.large !\n");
                exit(1);
	}
  /* fin ouverture fic */
  
  /* création tube */
  if (pipe(tube) == -1) {
    perror("Creation pipe impossible");
    exit(1);
  }

  /* création fils 1 */
  if ((pid1=fork()) == -1){
    perror("Creation fils 1");
    exit(1);
  }

  if (pid1 == 0) {
    /* Code du fils 1 */
       /* Ouverture fichier*/

	close(tube[0]); // fermeture tube fils en lecture
	while((nb=read(fd, buf, sizeof(buf)))>0)
  	{
		write(tube[1],buf,nb);  // on écrit en entrée du tube
  	}
    exit(0);
  }

  /* Code du pere */
  /* création fils 2 */
  if ((pid2=fork()) == -1)
  {
    perror("Creation fils 2");
    exit(1);
  }

 if (pid2 == 0) {
    /* Code du fils 2 */
    close(tube[1]); // ferme écriture
    while((nb=read(tube[0], buf, sizeof(buf)))>0) // le fils2 lit la sortie
  	{
        for (i=0;i<nb;i++)
		{
			if (buf[i] >='a' && buf[i]<='z')
			{
				buf[i]-=ecart;
			}
		}
		write(1,buf,nb);
  	}

    exit(0);
  }

/* Code du pere */

 close(fd);
 wait(&status);
 wait(&status);


  return(0);
}

