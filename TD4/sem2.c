/* IPC: Memoire partagee + semaphore - exemple_sem1b.c */

/* 2 programmes:
- exemple_sem1.c: cree ou ouvre le segment de memoire partagee,
                  cree ou ouvre le semaphore,
                  prend le semaphore
                  inscrit un message
                  libere le semaphore invitation
                  se met en sommeil en attente de reception d'une reponse
                  affiche la reponse
- exemple_sem1b.c:  cree ou ouvre le segment de memoire partagee,
                   cree ou ouvre le semaphore,
                   ouvre le segment de memoire
		   se met en sommeil en attente de reception d'une invitation
                   affiche le message recu
                   ecrit la reponse
                   libere le semaphore reponse
*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <errno.h>

int main(void) {

  int cle, cleSem, id, idSem;
  char *adresse;  /* adresse du segment de memoire partagee */

  union semun {  /* pour semctl() */
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;  /* array for GETALL, SETALL */
    struct seminfo *__buf;      /* buffer for IPC_INFO */
  }arg;

  struct sembuf semBuf;  /* pour semop() */
  unsigned short int tab[2];

  /* Tirage des cles */

  if ((cle=ftok("exemple_sem1.c", 1))==-1){
    perror("ftok 1");
    exit(1);
  }

  if ((cleSem=ftok("exemple_sem1.c", 2))==-1){
    perror("ftok 2");
    exit(1);
  }

  /* Ouverture du segment de memoire partagee */

  if ((id = shmget(cle, 80, 0600|IPC_CREAT))==-1) {
    perror("Ouverture memoire partagee");
    exit(1);
  }

  /* Attachement de la zone de memoire a l'espace memoire du processus */

  if ((int)(adresse = shmat(id, NULL, 0)) == -1) {
    perror("Attachement memoire partagee");
    shmctl(id, IPC_RMID, NULL);
    exit(1);
  }

  /* Creation d'un ensemble de semaphores */

  if ((idSem = semget(cleSem, 2, IPC_CREAT|IPC_EXCL|0600)) != -1) {

    /* Creation de l'ensemble, on initialise les semaphores a 0*/

    tab[0]=0;  /* invitation */
    tab[1]=0;  /* reponse */
    arg.array=tab;
    if (semctl(idSem, 0, SETALL, arg)==-1) {
      perror("exemple_sem1b:init semaphore");
      semctl(idSem, 0, IPC_RMID);
      shmctl(id, IPC_RMID, NULL);
      exit(1);
    }
  }
  else {
    /* erreur: soit l'ensemble existe deja, dans ce cas, on l'ouvre sans initialisation
       soit erreur reelle*/

    if (errno == EEXIST) {   /* l'ensemble est deja cree */
      if ((idSem = semget(cleSem, 2, IPC_CREAT|0600)) == -1) {
	perror("Obtention semaphore 1");
	shmctl(id, IPC_RMID, NULL);
	exit(1);
      }
    }
    else {  /* erreur reelle */
      perror("Creation semaphore 2");
      shmctl(id, IPC_RMID, NULL);
      exit(1);
    }
  }

  /* Attente de modification du segment de memoire */

  semBuf.sem_num=0;
  semBuf.sem_op=-1;
  semBuf.sem_flg=SEM_UNDO;
  if (semop(idSem, &semBuf, 1)==-1) {
    perror("Attente invitation");
    semctl(idSem, 0, IPC_RMID);
    shmctl(id, IPC_RMID, NULL);
    exit(1);
  }

  /* Affichage du message recu */

  printf("Message recu: %s\n", adresse);

  /* Ecriture de la reponse */

  strcpy(adresse,"Ce soir");

  /* Liberation du semaphore reponse */
  semBuf.sem_num=1;
  semBuf.sem_op=1;
  semBuf.sem_flg=0;

  if (semop(idSem, &semBuf, 1)==-1) {
    perror("Liberation semaphore invitation");
    semctl(idSem, 0, IPC_RMID);
    shmctl(id, IPC_RMID, NULL);
    exit(1);
  }

  /* Detachement du segment de memoire de l'espace du processus */

  shmdt(adresse);

  sleep(2);

  /* Suppression des IPC */
  semctl(idSem, 0, IPC_RMID);
  shmctl(id, IPC_RMID, NULL);

  return 0;
}
