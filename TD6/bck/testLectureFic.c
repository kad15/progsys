/* BELDJILALI IENAC15_SITA  TD6 */
/* Conversion multi htread avec supervision */
/* gcc -Wall -W -lpthread -o minmaj6 minmaj6.c */

#define _REENTRANT
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define N 8
#define TAILLE_BUF 512


int _fd2;
unsigned long _idThread=0;

typedef struct elt elt;
struct elt
{
    int taille;
    char donnee[TAILLE_BUF];
};
int iecr=0,ilect=0;
static elt tampon[N];

pthread_cond_t _cond = PTHREAD_COND_INITIALIZER; /* Création de la condition */
pthread_mutex_t _m = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */



pthread_mutex_t _mutex_fichier = PTHREAD_MUTEX_INITIALIZER;



void *prod(void* arg)  /* lit dans le fichier et ecrit dans le tampon circulaire*/
{

    int * ptr_fd = (int*)arg;
   // do
    //{
    do
    {
        pthread_mutex_lock(&_mutex_fichier);
        tampon[0].taille=read(*ptr_fd, tampon[0].donnee, sizeof(tampon[0].donnee));
        write(_fd2,tampon[0].donnee, tampon[0].taille);
        printf("\nthread %lu ecrit dans le fichier\n",pthread_self());
       // if(pthread_equal(_idThread,pthread_self())==0)
        //    printf("\nchangement de thread %lu -> %lu  !\n",_idThread,pthread_self());
        _idThread = pthread_self();

        pthread_mutex_unlock(&_mutex_fichier);
        sleep(1);

    } while(tampon[0].taille>0);
    pthread_exit(NULL);
}



int main()
{

    int fd;
    //tampon[0].taille = 999;


    pthread_t idthreadProducteur1, idthreadProducteur2; /* identifiant des threads */

// Si un nom de fichier est fourni en paramètre, on ouvre ce fichier
// et on lui associe le descripteur sortie standard


        if ((fd=open("fic.large", O_RDONLY))==-1)
        {
            perror("Oooops ");
            exit(1);
        }


// ouverture fichier de sortie fic.txt dans le quel on copie le fichier fic.large

 if ((_fd2=open("fic.txt", O_WRONLY|O_CREAT|O_TRUNC,0644))==-1)
        {
            perror("main fic.txt : ");
            exit(1);
        }

    /* creation threadEcriture1*/
    if (pthread_create(&idthreadProducteur1, NULL, prod, &fd)!=0)
    {
        printf("erreur creation threadEcriture1\n");
        exit(1);
    }

    /* creation threadEcriture1*/
    if (pthread_create(&idthreadProducteur2, NULL, prod, &fd)!=0)
    {
        printf("erreur creation threadEcriture2\n");
        exit(1);
    }



    /* attente de la terminaison des threads */
    pthread_join(idthreadProducteur1, NULL);
    pthread_join(idthreadProducteur2, NULL);


    close(_fd2);
    close(fd);
    return 0;
}





