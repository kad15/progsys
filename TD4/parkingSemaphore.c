/* ex_thread8.c */
/* Cohorte : ouverture barriere d un parking */
/* cf support cours Programmation Concurrente */
/* gcc -Wall -lpthread -o thread8 ex_thread8.c */

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void detectionEntree(); /* fonction bloquante tant qu aucune voiture ne se presente */
void detectionSortie(); /* fonction bloquante tant qu aucune voiture ne se presente */

void ouvertureEntree(); /* fonction actionnant l ouverture d une barriere */
void ouvertureSortie(); /* fonction actionnant l ouverture d une barriere */

sem_t _nbLibr; /* declaration du semaphore nb places libres  */


void *entree(void *arg){

  while(1){
    /* Attente detection voiture */
    detectionEntree();

    /*P(nbLibres) : bloquant si pas de place */
    if (sem_wait(&_nbLibr)==-1){
      perror("Entree:erreur wait:");
      pthread_exit(NULL);
    }
    /* ouverture de la barriere */
    ouvertureEntree();

  } /*fin while */

}

void *sortie(void *arg){

  while(1){
   /* Attente detection voiture */
    detectionSortie();

    /*V(nbLibres) : incremente le nombre de places libres */
    if (sem_post(&_nbLibr)==-1){
      perror("Sortie:erreur post:");
      pthread_exit(NULL);
    }
    /* ouverture de la barriere */
    ouvertureSortie();

  }/*fin while */
}

int main(void){

  pthread_t idThreadE, idThreadS; /* identifiant des threads */

  /* initialisation semaphore au nombre de places du parking */
  if (sem_init(&_nbLibr, 0, 50)==-1){
    perror("main:erreur init nbLibr:");
    exit(1);
  }

  /* creation thread entree */
  if (pthread_create(&idThreadE, NULL, entree, NULL)!=0){
    printf("erreur creation thread entrees\n");
    exit(1);
  }

  /* creation thread sortie */
  if (pthread_create(&idThreadS, NULL, sortie, NULL)!=0){
    printf("erreur creation thread sorties\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThreadE, NULL);
  pthread_join(idThreadS, NULL);

/* suppression du semaphores */
  sem_destroy(&_nbLibr);

  return 0;
}
