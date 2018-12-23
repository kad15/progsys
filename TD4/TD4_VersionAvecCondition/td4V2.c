/* Affichage a l'ecran du contenu d'un fichier dont le nom est passe en parametre */
#define _REENTRANT
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


/* BELDJILALI IENAC15_SITA  TD4 version sans sémaphores */
/* Execution concurrente de 2 threads client serveur */
/* gcc -Wall -W -lpthread -o td4 ex_td4.c */
#define NB_REQUETE 50
float data; /* espace mémoire où le serveur écrit la donnée */


pthread_cond_t requete = PTHREAD_COND_INITIALIZER; /* Création de la requete */
pthread_cond_t reponse = PTHREAD_COND_INITIALIZER; /* Création de la réponse */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */
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

    for(i=0; i<NB_REQUETE; i++)
    {

        pthread_mutex_lock (&mutex); /* On verrouille le mutex */
        printf("\nEmission requête : %d", i);
        pthread_cond_signal (&requete); /* On émet la requete*/
        pthread_cond_wait (&reponse, &mutex); /* On attend que la donnée soit dispo */
        printf("\n Valeur affichée par le client : %f", data);
        pthread_mutex_unlock (&mutex); /* On déverrouille le mutex */
    }
    pthread_exit(NULL); /* Fin du thread */
}


//
void* threadServeur(void* fichier)
{
    while(1) /* Boucle infinie */
    {
        pthread_mutex_lock(&mutex); /* On verrouille le mutex */

        pthread_cond_wait (&requete, &mutex); /* On attend que la "condition" requete soit envoyée */
        //printf("\nLecture d'un nombre dans le fichier.");
        fscanf(fichier, "%f", &data);
        printf("\n Valeur lue par le serveur : %f", data);
        pthread_cond_signal (&reponse); /* On délivre le signal : réponse prête */
        pthread_mutex_unlock(&mutex); /* On déverrouille le mutex */
    }

}



int main (void)
{
    pthread_t idThreadServeur;
    pthread_t idThreadClient;
    char * nomfic = "ficNombres.txt";
    FILE* fichier = NULL;

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
    return 0;
}



