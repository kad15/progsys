/* BELDJILALI IENAC15_SITA  TD8 */
/* Suivi du déroulement d'une application non interactive */
/* gcc -Wall -W -o calcul calcul.c */

#include <stdio.h>   /* pour printf() */
#include <stdlib.h>  /* pour exit() */
#include <string.h>  /* pour strcmp() */
#include <sys/ipc.h> /* pour ftok() */
#include <sys/shm.h> /* shmget() shmat()*/
#include <signal.h>  /* SIGUSR etc*/
#include <unistd.h>


typedef struct {
    pid_t pid ; // pid de l'appli
    char message[80]; // texte d'info
    float valeur;  // résultat intermédiaire de calcul
} stuctureInfoAppli;

stuctureInfoAppli * adr; // adresse mémoire partagée



int main()
{


    key_t key = ftok("calcul",1);
    int mem_ID; //	identificateur du segment de mémoire partagée associé à key

    mem_ID = shmget(key, sizeof(stuctureInfoAppli), 0600 | IPC_CREAT);
    adr = shmat(mem_ID, NULL, 0);

    adr->pid = getpid();
    strcpy(adr->message, "coucou, je calcule !");

    adr->valeur = 0;
    while(1) {
        adr->valeur += 1.0;
        printf("%f\n",adr->valeur);
        if ( adr->valeur > 15000000) adr->valeur=0;
    }


    shmdt(adr);


    return 0;
}
