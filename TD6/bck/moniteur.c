#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 8
typedef struct elt elt;
struct elt
{
    int taille;
    char donnee[TAILLE_BUF];
};


void *moniteur(void *arg)
{

int nV, nP;

pthread_cond_t _Cond_nonPlein = PTHREAD_COND_INITIALIZER;
pthread_mutex_t _mut_nonPlein = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t _Cond_nonVide = PTHREAD_COND_INITIALIZER;
pthread_mutex_t _mut_nonVide = PTHREAD_MUTEX_INITIALIZER;

int iecr=0,ilect=0; // q t
elt fifo[N];
int * ptr_fd = (int*)arg;

void *producteur(void* arg)  /* lit dans le fichier et ecrit dans la file fifo circulaire*/
{
    if (nV==0)
    {
    pthread_mutex_lock(&mutex); /* On verrouille le mutex */
    pthread_cond_wait (&_Cond_nonPlein, &_mut_nonPlein); /* On attend que la condition soit remplie */
    printf("\ntampon plein.");
    pthread_mutex_unlock(&_mut_nonPlein);
    }
    /* nV >0 */
    nV--;
    fifo[n].taille=read(*ptr_fd, fifo[n].donnee, sizeof(fifo[n].donnee));
}

}
