/* BELDJILALI IENAC15_SITA  TD5 */
/* schéma producteur-consommateur */
/* gcc -Wall -W -lpthread -o minmaj5 minmaj5.c */

#define _REENTRANT
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


#define N 8
#define TAILLE_BUF 512

typedef struct elt elt;
struct elt
{
    int taille;
    char donnee[TAILLE_BUF];
};
int iecr=0,ilect=0;
static elt fifo[N];



sem_t semCaseVide, semCasePleine ;

pthread_mutex_t _mutex_iecr = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutex_ilect = PTHREAD_MUTEX_INITIALIZER;

/* producteur : ecrit dans la file fifo circulaire de 0 à N-1 avant de recommencerf à partir de 0*/
/* le producteur ne doit pas écrire si la file fifo est plein */
/* le producteur et le consommateur ne doivent pas travailler dans une même case*/
/* le producteur écrit dans des case vides ; ce qui se traduit par P(semCaseVide) et fournit des cases pleines => V(semCasePleines)*/
void *threadEcriture(void* fd)  /* lit dans le fichier et ecrit dans la file fifo circulaire*/
{
    fd = fd;
    int  n;
//while(1){
    if (sem_wait(&semCaseVide)==-1)  // si N>0 fait N-- et exécute la suite du code
    {
        perror("caseVide:erreur wait:");
        pthread_exit(NULL);
    }


    pthread_mutex_lock(&_mutex_iecr);
    n=iecr % N;
    fifo[n].taille=read(0, fifo[n].donnee, fifo[n].taille);
    iecr++;
    pthread_mutex_unlock(&_mutex_iecr);

    if (sem_post(&semCasePleine)==-1)
    {
        perror("casePleine:erreur post:");
        pthread_exit(NULL);
    }

//}
    return(0);
}

/* consommateur lit dans la file circulaire de 0 à N-1 avant de recommencerf à partir de 0*/
/* le consommateur ne doit pas faire de lecture si la file fifo est vide */
/* le consommateur lit des cases pleines et fournit des cases vides*/
void *convAffiche(void* fd)  /* convertit en capital le texte et l'affiche sur stdout */
{
    fd=fd;
    int i, n, ecart = 'a' - 'A';


//while(1)
//{
    if (sem_wait(&semCasePleine)==-1)  // si N>0 fait N-- et exécute la suite du code
    {
        perror("casePleine:erreur wait:");
        pthread_exit(NULL);
    }


    pthread_mutex_lock(&_mutex_ilect);
    n=ilect % N;
    for (i=0; i<fifo[n].taille; i++)
    {
        if (fifo[n].donnee[i] >='a' && fifo[n].donnee[i] <='z')
        {
            fifo[n].donnee[i] -=ecart;
        }
    }
    write(1,fifo[n].donnee, fifo[n].taille);
    ilect++;
    pthread_mutex_unlock(&_mutex_ilect);


    if (sem_post(&semCaseVide)==-1)
    {
        perror("caseVide:erreur post:");
        pthread_exit(NULL);
    }
//}
    return (0);
}

int main(int argc, char* argv[])
{

    int fd;
    fifo[0].taille = 999;


    pthread_t idthreadEcriture1, idthreadEcriture2, idThreadConvAffiche; /* identifiant des threads */

    if (sem_init(&semCaseVide, 0, 8)==-1)/* init sémaphore 1ere 0 : sem interne au processus 2nd 0 : valeur = 0   */
    {
        perror("erreur init semaphore");
        exit(1);
    }
    if (sem_init(&semCasePleine, 0, 0)==-1)
    {
        perror("erreur init semaphore");
        exit(1);
    }

// Si un nom de fichier est fourni en paramètre, on ouvre ce fichier
// et on lui associe le descripteur sortie standard

    if (argc==2)
    {
        if ((fd=open(argv[1], O_RDONLY))==-1)
        {
            perror("Ouverture");
            exit(1);
        }
        close(0);
        dup(fd);
        close(fd); // le descripteur num 1 pointe sur le fichier donné en argument

    }


    /* creation threadEcriture1*/
    if (pthread_create(&idthreadEcriture1, NULL, threadEcriture, NULL)!=0)
    {
        printf("erreur creation threadEcriture1\n");
        exit(1);
    }

    /* creation threadEcriture1*/
    if (pthread_create(&idthreadEcriture2, NULL, threadEcriture, NULL)!=0)
    {
        printf("erreur creation threadEcriture2\n");
        exit(1);
    }

    /* creation thread conversion*/
    if (pthread_create(&idThreadConvAffiche, NULL, convAffiche, NULL)!=0)
    {
        printf("erreur creation thread Conversion\n");
        exit(1);
    }



    /* attente de la terminaison des threads */
    pthread_join(idthreadEcriture1, NULL);
    pthread_join(idthreadEcriture2, NULL);
    pthread_join(idThreadConvAffiche, NULL);


    /* suppression des semaphores */
    printf("\nDestruction sémaphores et fermeture fichier");
    sem_destroy(&semCaseVide);
    sem_destroy(&semCasePleine);

    return 0;
}





