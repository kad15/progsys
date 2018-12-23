#define _REENTRANT
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

/* gcc -Wall -W -lpthread -o moniteur_posix moniteur_posix.c */

/* définition du tampon */
#define N 10
/* Nb de cases du tampon */
#define NbMess 20 /* Nb de messages échangés */
int NbPleins=0;
int tete=0, queue=0;
int tampon[N];
/* définition des conditions et du mutex */
pthread_cond_t vide = PTHREAD_COND_INITIALIZER;
pthread_cond_t plein = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid[2];

void Deposer(int m)
{
    pthread_mutex_lock(&mutex);
    if(NbPleins == N) pthread_cond_wait(&plein, &mutex);
    tampon[queue]=m;
    queue=(queue+1)%N;
    NbPleins++;
    pthread_cond_signal(&vide);
    pthread_mutex_unlock(&mutex);
}

int Prelever(void)
{
    int m;
    pthread_mutex_lock(&mutex);
    if(NbPleins ==0) pthread_cond_wait(&vide, &mutex);
    m=tampon[tete];
    tete=(tete+1)%N;
    NbPleins--;
    pthread_cond_signal(&plein);
    pthread_mutex_unlock(&mutex);
    return m;
}






void * Prod(void * k)
/********** PRODUCTEUR */
{
    k=k;
    int i;
    int mess;
    srand(pthread_self());
    for(i=0; i<=NbMess; i++)
    {
        usleep(rand()%10000); /* fabrication du message */
        mess=rand()%1000;
        Deposer(mess);
        printf("Mess depose: %d\n",mess);
    }
    pthread_exit(NULL);
}


void * Cons(void * k)
/********** CONSOMMATEUR */
{
    k=k;
    int i;
    int mess;
    srand(pthread_self());
    for(i=0; i<=NbMess; i++)
    {
        mess=Prelever();
        printf("\tMess preleve: %d\n",mess);
        usleep(rand()%1000000); /* traitement du message */
    }
    pthread_exit(NULL);
}


int main()
{

    //pthread_mutex_init(&mutex,0);
    //pthread_cond_init(&vide,0);
    //pthread_cond_init(&plein,0);
    /* creation des threads */
    pthread_create(tid, 0, (void * (*)()) Prod, NULL);
    pthread_create(tid+1, 0, (void * (*)()) Cons, NULL);
// attente de la fin des threads
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
// libération des ressources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vide);
    pthread_cond_destroy(&plein);

    exit(0);
}
