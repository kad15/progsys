/* BELDJILALI IENAC15_SITA  TD8 */
/* Suivi du déroulement d'une application non interactive */
/* gcc -Wall -W -lpthread -o suivi suivi.c */


#include <fcntl.h> /* pour read et open*/
#include <stdio.h> /* pour perror et printf */
#include <unistd.h>  /* pour fork() */
#include <stdlib.h>  /* pour exit() */
#include <signal.h>  
#include <limits.h>  /* pour ULONG_MAX */

#define FALSE 0
#define TRUE 1


void fonction_usr1(int num){
}




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



key_t key;
key = ftok("applicationTD3",1);


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


