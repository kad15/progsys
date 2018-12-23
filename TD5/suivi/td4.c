/* BELDJILALI IENAC15_SITA  TD4  version thread client serveur avec sémaphores*/
/* Execution concurrente de 2 threads client serveur */
/* gcc -Wall -W -lpthread -o td4 ex_td4.c */

#define _REENTRANT
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>



sem_t semRep, semReq;

float _data; /* espace mémoire où le serveur écrit la donnée et le client lit*/


/*
thread client : fait 50 fois le traitement suivant :
- émission d'une requête au serveur
- récup données envoyées par le serveur
- affichage de ces données
 */

void* threadClient(void* arg)
{
    int i = 0;
    arg=arg;

    for(i=0; i<50; i++)
    {
        if (sem_post(&semReq)==-1)  // 1 - le thread client incrémente le semRep val == 1 et la sute s'execute
            {perror("requete:erreur post:"); pthread_exit(NULL);}

        printf("\nEmission requête : %d", i); // le client "envoie sa requete" et attend

        if (sem_wait(&semRep)==-1) // 2 - le thread client attend la dispo d'un jeton du sem réponse (val sem ==0), il est mis en attente dans la file.
            {perror("reponse:erreur wait:"); pthread_exit(NULL);}

        printf("\n valeur lue et affichée par le client %f : ", _data);

    }

    pthread_exit(NULL); /* Fin du thread */
}


//
void* threadServeur(void* fichier)
{
    while(1) /* Boucle infinie */
    {
        if (sem_wait(&semReq)==-1) // 3 - le thread serveur est alors élu par l'OS. un jeton est disponible.
                                    // semReq est décrémenté val ==0 et la suite s'éxecute
            {perror("requete:erreur wait:"); pthread_exit(NULL);}

        fscanf(fichier, "%f", &_data);
        printf("\n valeur lue par le serveur dans le fichier %f : ", _data);

        if (sem_post(&semRep)==-1) // 4 - le thread serveur incrémente la valeur du sem reponse jeton++ val==1
            {perror("reponse:erreur post:"); pthread_exit(NULL);}

    }
    pthread_exit(NULL); /* Fin du thread */
}



int main (void)
{
    pthread_t idThreadServeur;
    pthread_t idThreadClient;
    char * nomfic = "ficNombres.txt";
    FILE* fichier = NULL;

    if (sem_init(&semRep, 0, 0)==-1)/* init sémaphore 1ere 0 : sem interne au processus 2nd 0 : valeur = 0   */
        {perror("erreur init semaphore"); exit(1);}
    if (sem_init(&semReq, 0, 0)==-1)
        {perror("erreur init semaphore"); exit(1);}



    fichier = fopen(nomfic, "r");
    if (fichier == NULL)
    {
        perror("ouverture fichier");
        exit(1);
    }


    pthread_create (&idThreadServeur, NULL, threadServeur, (void*)fichier);
    pthread_create (&idThreadClient, NULL, threadClient, (void*)NULL); /* Création des threads */


    pthread_join (idThreadServeur, NULL);
    pthread_join (idThreadClient, NULL); /* Attente de la fin des threads */
    fclose(fichier);

    /* suppression des semaphores */
    sem_destroy(&semRep);
    sem_destroy(&semReq);
    return 0;
}



