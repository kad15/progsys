/* BELDJILALI IENAC15_SITA  TD8 */
/* Suivi du déroulement d'une application non interactive */
/* gcc -Wall -W -o suivi suivi.c */

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

void fin(int numSig)
{
    numSig = numSig;
    printf("execution de fin() sur réception signal SIGUSR2.\n");
    printf("arrêt du processus de calcul de pid est égal à : %d\n", adr->pid);

    if (kill (adr->pid, 9)==-1) {
        perror("Erreur kill du processus de calcul. Ce dernier est sans doute déjà terminé.");
        exit(1);
    }

}


void info(int numSig)
{
    numSig = numSig;
    printf("execution de info() sur réception signal SIGUSR1.\n");
    printf("Le pid est égal à : %d\n", adr->pid);
    printf("Le message est : %s\n", adr->message);
    printf("La valeur intermediaire du calcul est : %f\n", adr->valeur);

}

int main(int argc, char * argv[])
{


    /* On élimine les saisies incorrectes de l'utilisateur. */
    if (    !(argc == 2 && (strcmp(argv[1],"info")==0 || strcmp(argv[1],"fin")==0))    ) {
        printf("ATTENTION PRECISER L'ACTION A EFFECTUER : ./suivi info ou ./suivi fin  pour superviser les calculs ou les terminer.\n");
        exit(0);
    }




    /* 1 - récupération de la clé de la zone de mémoire */
    key_t key;
    if ((key = ftok("calcul",1))==-1) {
        perror("ftok");
        exit(1);
    }



    /* 2 - récupération de l'id de la zone mémoire associée à key */
    int mem_id;
    if ((mem_id= shmget(key, sizeof(stuctureInfoAppli), 0444)) == -1) {
        perror("shmget. Verifier que le programme ./calcul est bien lance !");
        exit(1);
    }
    /* 3 - récupération de l'adresse */
    if ((adr = shmat(mem_id, NULL, SHM_RDONLY)) == (void *) -1) {
        perror("shmat");
        exit(1);
    }



    if(strcmp(argv[1],"info")==0) {
        /* Installation du handler pour SIGUSR2. */
        signal(SIGUSR1, info);


        /* on envoie au processus "suivi" le signal sigusr1 ce qui déclenche info()*/
        if (kill (getpid(), SIGUSR1)==-1) {
            perror("Erreur envoi signal USR1 au processus 'suivi'.");
            exit(1);
        }

    }



    if(strcmp(argv[1],"fin")==0) {
        /* Installation du handler pour SIGUSR2. */
        signal(SIGUSR2, fin);
        /* on envoie au processus "suivi" le signal sigusr2 ce qui déclenche fin()*/
        if (kill (getpid(), SIGUSR2)==-1) {
            perror("Erreur envoi signal USR2 au processus 'suivi'.");
            exit(1);
        }
        /* marquage pour destruction zone mémoire partagée */
        shmctl(mem_id,IPC_RMID,NULL);

    }

    // la mémoire partagee est détachée de l'espace mémoire du processus de suivi
    if ((shmdt(adr)) == -1) {
        perror("shmdt");
        exit(1);
    }


    return(0);
}


