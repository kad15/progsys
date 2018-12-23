/* IPC: Memoire partagee - exemple_shm1.c */

/* 2 programmes:
- exemple_shm1.c: cree un segment de memoire partagee,
                  y inscrit un message
                  se met en sommeil en attente de reception d'une reponse
                  affiche la reponse
- exemple_shm1b.c: ouvre le segment de memoire
                   affiche le message recu
                   ecrit la reponse

Attention : exemple_shm1 doit etre lance en premier. Pas de gestion de la synchronisation.

*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {

  int cle, id;
  char *adresse;  /* adresse du segment de memoire partagee */
  char msg[80]="Quand venez vous diner?";

/* Tirage de cle */

  if ((cle=ftok("exemple_shm1.c", 1))==-1){
    perror("ftok");
    exit(1);
  }

/* Creation d'un segment de mmoire partagee de 80 octets */

  if ((id = shmget(cle, 80, 0600|IPC_CREAT))==-1) {
    perror("Creation memoire partagee");
    exit(1);
  }

/* Attachement de la zone de memoire a l'espace memoire du processus */

  if ((int)(adresse = shmat(id, NULL, 0)) == -1) {
    perror("Attachement memoire partagee");
    shmctl(id, IPC_RMID, NULL);  /* Suppression du segment */
    exit(1);
  }

  /* Ecriture du message */

  strcpy(adresse, msg);

/* Attente de modification du segment de memoire */

  while (strncmp(msg, adresse, 80)==0)
    sleep(1);

/* Affichage de la reponse */

  printf("Ils viennent diner %s\n", adresse);

/* Detachement du segment de memoire de l'espace du processus */

  shmdt(adresse);

/* Suppression du segment */

  shmctl(id, IPC_RMID, NULL);

  return 0;
}


/* IPC: Memoire partagee - exemple_shm1b.c */

/* 2 programmes:
- exemple_shm1.c: cree un segment de memoire partagee,
                  y inscrit un message
                  se met en sommeil en attente de reception d'une reponse
                  affiche la reponse
- exemple_shm1b.c: ouvre le segment de memoire
                   affiche le message recu
                   ecrit la reponse

Attention : exemple_shm1 doit etre lance en premier. Pas de gestion de la synchronisation.

*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {

  int cle, id;
  char *adresse;  /* adresse du segment de memoire partagee */

  /* Tirage de cle */

  if ((cle=ftok("exemple_shm1.c", 1))==-1){
    perror("ftok");
    exit(1);
  }

  /* Ouverture du segment de mmoire partagee */

  if ((id = shmget(cle, 80, 0))==-1) {
    perror("Ouverture memoire partagee");
    exit(1);
  }

  /* Attachement de la zone de memoire a l'espace memoire du processus */

  if ((int)(adresse = shmat(id, NULL, 0)) == -1) {
    perror("Attachement memoire partagee");
    exit(1);
  }

  /* Affichage du message recu */

  printf("Message recu: %s\n", adresse);

  /* Ecriture de la reponse */

  strcpy(adresse,"Ce soir");

  sleep(1);

/* Detachement du segment de memoire de l'espace du processus */

  shmdt(adresse);

  return 0;
}
