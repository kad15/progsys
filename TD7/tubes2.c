/* BELDJILALI IENAC15_SITA  TD7 */
/* transfert  à travers des tubes avec utilisation de minmaj */
/* gcc -Wall -W -lpthread -o tubes2 tubes2.c */

//#define _REENTRANT
#include <fcntl.h> /* pour read et open*/
#include <stdio.h> /* pour perror et printf */
#include <unistd.h>  /* pour fork() */
#include <stdlib.h>  /* pour exit() */
#include <sys/wait.h> /* pour wait */
#include <signal.h>  /* pour sleep */
//#include <string.h>

#define FALSE 0
#define TRUE 1

int main(int argc,  char *argv[])
{
    int pid1=0, pid2=0;
    int status;  /* pour code retour wait()*/
    int fd, nb;
    char buf[512];


    int tube[2];


    if (argc==2) {
        if ((fd=open(argv[1], O_RDONLY))==-1) {
            perror("Ouverture");
            exit(1);
        }
    } else {
        printf("Erreur ! Veuillez préciser un nom de fichier à lire ici fic.large !\n");
        exit(1);
    }



    /* création tube */
    if (pipe(tube) == -1) {
        perror("Creation pipe impossible");
        exit(1);
    }
    /* création fils 1 */
    if ((pid1=fork()) == -1) {
        perror("Creation fils 1");
        exit(1);
    }

    if (pid1 == 0) {
        /* Code du fils 1 */

        close(tube[0]); // fermeture tube fils en lecture
        while((nb=read(fd, buf, sizeof(buf)))>0) {
            write(tube[1],buf,nb);  // on écrit en entrée du tube

            printf("dans pid1 after write ds tube");
        }
        exit(0);
    }


    /* Code du pere */
    /*************************************************/
    /* création fils 2 */
    if ((pid2=fork()) == -1) {
        perror("Creation fils 2");
        exit(1);
    }


    /* Code du pere */
    if (pid2 == 0) {
        /* Code du fils 2 */
        close(tube[1]); // ferme écriture tube fils car non utilisé
        close(0); // ferme l'entree standard
        dup(tube[0]); // le descripteur du tube en lecture est copié dans l'entrée standard
        // l'entrée standard de minmaj
        close(tube[0]); // on libère le descripteur lecture du tube
        /* On lance le programme  */
        char *arg[] = {"minmaj", argv[1], NULL };
        if (  (execv("minmaj",arg)) == -1  ) {
            perror("execv");
            exit(1);
        }
        exit(0);
    }

    close(fd);
    wait(&status);
    wait(&status);


    return(0);
}

