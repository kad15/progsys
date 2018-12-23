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
