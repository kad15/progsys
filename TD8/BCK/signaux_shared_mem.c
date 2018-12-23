/* Gestion des signaux - Posix */
 
/* Controle d'avancemant d'une application:
Le programme principal effectue un calcul
- Blocage de CTRL-C
- Reception de SIGUSR1 (16 sous Solaris) : Affichage du resultat intermediaire et suite du calcul
- Reception de SIGUSR2 (17 sous Solaris) : Affichage du resultat et fin du programme
*/

#include <stdio.h>
#include <limits.h>  /* pour ULONG_MAX */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long somme=0;

void espion(int numSig) {
  printf("Somme = %lu\n", somme);
  switch(numSig){
  case SIGUSR1:
    break;
  case SIGUSR2:
    exit(0);
  }
}

int main(void) {

  sigset_t masque;
  struct sigaction act;

  /* Application du masque de blocage des signaux : on interdit au processus de recevoir CTRL-C (SIGINT). Tous les autres signaux sont autoris�s */

  sigemptyset(&masque);
  sigaddset(&masque, SIGINT);
  sigprocmask(SIG_SETMASK, &masque, NULL);

/* Installation du handler espion() pour SIGUSR1 et SIGUSR2. On interdit la reception de tout  signal durant l'execution de la fonction espion() */

  act.sa_handler=espion;
  act.sa_flags=0;
  sigfillset(&(act.sa_mask));
  sigaction(SIGUSR1, &act, NULL);
  sigaction(SIGUSR2, &act, NULL);

/* Traitement */

  printf("Je suis le processus numero: %d\n", getpid());
  while(somme<ULONG_MAX) {
    somme++;
  }
  printf("Calcul termine: %lu", somme);

  return(0);
}


*************************************


/* Gestion des signaux - ANSI C */
 
/* Controle d'avancemant d'une application:
Le programme principal effectue un calcul
- Ignorance de CTRL-C
- Reception de SIGUSR1 (16 sous Solaris) : Affichage du resultat intermediaire et suite du calcul
- Reception de SIGUSR2 (17 sous Solaris) : Affichage du resultat et fin du programme
*/

#include <stdio.h>
#include <limits.h>  /* pour ULONG_MAX */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long somme=0;

void espion(int numSig) {
  printf("Somme = %lu\n", somme);
  switch(numSig){
  case SIGUSR1:
    signal(SIGUSR1, espion); /* On rearme le signal */
    break;
  case SIGUSR2:
    exit(0);
  }
}

int main(void) {

  /* on interdit au processus de recevoir CTRL-C (SIGINT). */
  signal(SIGINT, SIG_IGN);

/* Installation du handler espion() pour SIGUSR1 et SIGUSR2. */

  signal(SIGUSR1, espion);
  signal(SIGUSR2, espion);

/* Traitement */

  printf("Je suis le processus numero: %d\n", getpid());
  while(somme<ULONG_MAX) {
    somme++;
  }
  printf("Calcul termine: %lu", somme);

  return(0);
}


**********************************************

/* Gestion des signaux  - */
 
/*  Mise en place d'une temporisation - Utilisation d'un saut non local
Le programme principal attend 5 seconde la saisie d'une valeur
Quand le delai est depasse, il affiche un message et se termine

- Envoi du signal SIGALRM au bout de 5 secondes
- La fonction de gestion du signal renvoie au programme principal

*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <setjmp.h>

sigjmp_buf contexteAlarme;

void gestionAlarme(int numSig) {
  siglongjmp( contexteAlarme, 1); /* branchement a sigsetjmp() en retournant 1 */
}

int main(void) {

  int i;

/* Installation du handler gestionAlarme  pour SIGALRM */

  signal(SIGALRM,gestionAlarme); 

/* Traitement */

  printf("Vous avez 5 secondes pour entrer un nombre entier\n");

/* Memorisation du contexte d'execution */

  if (sigsetjmp(contexteAlarme, 1) == 0) { /* 1er passage */
    alarm(5);     /* Mise en place du timer */
    scanf("%d", &i);  
    alarm(0);  /* saisie effectuee, on annule le timer */
    printf("OK: %d saisi\n",i);
  }
  else {
    printf("Trop tard!!!\n");
  }

  return(0);
}


*****************************************************************
/* Exemples de tirage de cle */

#include <sys/ipc.h>
#include <stdio.h>

int main(void) {

  printf("%d\n", ftok("exemple_pipe.c", 'A'));   /* 1090916642 */
  printf("%d\n", ftok("exemple_pipe.c", 'G'));   /* 1191579938 */
  printf("%d\n", ftok("exemple_fork1.c", 'A'));  /* 1090916631 */
  printf("%d\n", ftok("exemple_fork1.c", 'G'));  /* 1191579927 */
  printf("%d\n", ftok("exemple_pipe.c", 1));     /* 17174818  */
  printf("%d\n", ftok("exemple_fork1.c", 1));    /* 17174807 */

  return 0;
}


****************************************************************



/* Exemples de tirage de cle */
/* Limites de ftok */

/* Le fichier lien_fork1 est un lien hard sur exemple_fork1.c (meme numero d'i-node) 
La cle tiree est la meme */

#include <sys/ipc.h>
#include <stdio.h>

int main(void) {

  printf("%d\n", ftok("exemple_fork1.c", 'A'));  /* 1090916631 */
  printf("%d\n", ftok("lien_fork1", 'A'));       /* 1090916631 */
  

  return 0;
}


*******************************************************************


* IPC: Memoire partagee - exemple_shm1.c */

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


****************************************************************


/* IPC: Files de messages - exemple_msg1b.c */

/* 2 programmes:
   - exemple_msg1.c: cree ou ouvre une file de messages, 
                     y inscrit 3 messages
                     se termine
   - exemple_msg1b.c: cree ou ouvre une file de messages
                      affiche les 3 messages lus
		      supprime la file
                      se termine

 la file de message peut etre cree par l'un des deux processus, La lecture est par defaut bloquante

*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {

  int cle , id, i;

  struct msg {
    long type;
    char data[80];
  }message;



/* Obtention de la cle */

  cle = ftok("exemple_msg1.c", 'A');

/* Creation d'une file de messages */
  
  if ((id = msgget(cle, 0600|IPC_CREAT))==-1) {
    perror("Creation file de messages");
    exit(1);
  }

  /* Ecriture de 3 messages */

  for (i=0;i<3;i++) {
    if (msgrcv(id, &message, 80, 0, 0)==-1){
      perror("Ecriture message");
      exit(1);
    }
    printf("%s\n", message.data);
  }

  /* Suppression de la file de messages */

  msgctl(id, IPC_RMID, NULL);

  return 0;
}


***********************************************************






