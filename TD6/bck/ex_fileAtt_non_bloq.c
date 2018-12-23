#define _REENTRANT
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// File d'attente implementee sous forme de buffer tournant (tableau de tailleMax elements de tailleElt octets)
// Supporte plusieurs "ecrivains" et plusieurs "lecteurs"

#define INFINITE 0

struct FILE_ATT {
    pthread_mutex_t m;
    pthread_cond_t vEcr;
    pthread_cond_t vLect;
    int nbOcc;
    int nbDispo;
    int iEcr;
    int iLect;
    char **laFile;
    int tailleElt;
    int tailleMax;
};

typedef struct FILE_ATT typ_FILE_ATT;

// Initialisations et allocation de la memoire. A appeller une seule fois avant utilisation de la file.
int initFile(typ_FILE_ATT *f, int tailleElt, int tailleMax)
{
// f : l adresse de la file
// taille element : nombre d octets d un element de la file
// taille max : nombre d elements max de la file
// renvoie 1 : OK, -1 : erreur (file d�j� initialisee, manque de memoire, erreurs techniques)
    int i;

    if (f->laFile != NULL)
        return -1;

    if (pthread_cond_init(&(f->vEcr), NULL)!=0)
        return -1;

    if (pthread_cond_init(&(f->vLect), NULL)!=0)
        return -1;

    if (pthread_mutex_init(&(f->m), NULL)!=0)
        return -1;

    if ((f->laFile = (char **)malloc(tailleMax*sizeof(char*))) == NULL)
        return -1;


    for (i=0; i<tailleMax; i++) {
        if ((f->laFile[i] = (char *)malloc(tailleElt*sizeof(char))) == NULL)
            return -1;
    }
    f->tailleElt = tailleElt;
    f->tailleMax = tailleMax;
    f->nbOcc = 0;
    f->nbDispo = tailleMax;
    f->iEcr = 0;
    f->iLect = 0;
    return 1;
}

// Ecriture d un element dans la file. les donnees sont ecrites a concurrence de la taille fixee pour l element
int ecriFile(typ_FILE_ATT *f, char* buf, int timeout)
{
// f : l adresse de la file
// buf : les donnees a ecrire dans la file (1 element)
// timeout : delai d'attente sur une ecriture en millisecondes (>0) ou attente infinie : INFINITE
// renvoie 1 : OK, 0 : delai d'attente depasse, -1 : erreur (file non initialisee, erreurs techniques)
    struct timespec t;
    int ret = 0;

    if (f->laFile == NULL)
        return -1;

    pthread_mutex_lock(&(f->m));

    if (!timeout) {
        while (f->nbDispo == 0) {
            pthread_cond_wait(&(f->vEcr), &(f->m));
        }
    } else {
        clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec += (timeout/1000);
        t.tv_nsec = 0;

        while (f->nbDispo == 0 && ret == 0) {
            ret=pthread_cond_timedwait(&(f->vEcr), &(f->m), &t);
            //printf("ret %d \n", ret); // 110 pour timeout echu sur Ubuntu 14.04 LTS
        }
        // if (ret == ETIMEDOUT)// non present sur la version Ubuntu
        if (ret != 0) { // timeout : on n ecrit rien dans la file
            pthread_mutex_unlock(&(f->m));
            return 0;
        }
    } // fin cas non bloquant

    memcpy(f->laFile[f->iEcr], buf, f->tailleElt);
    f->nbDispo--;
    f->nbOcc++;
    f->iEcr=(f->iEcr + 1)%f->tailleMax;
    pthread_cond_broadcast(&(f->vLect));
    pthread_mutex_unlock(&(f->m));

    return 1;

}

// Lecture d un element dans la file.
int lireFile(typ_FILE_ATT *f, char *buf, int timeout)
{
// f : l adresse de la file
// le buffer de recuperation des donnees � lire (1 element). Doit etre de taille >= � la taille fixee pour l element
// timeout : delai d'attente sur une lecture en millisecondes (>0) ou attente infinie : INFINITE
// renvoie 1 : OK, 0 : delai d'attente depasse, -1 : erreur (file non initialisee, erreurs techniques)

    struct timespec t;
    int ret = 0;

    if (f->laFile == NULL)
        return -1;

    pthread_mutex_lock(&(f->m));

    if (!timeout) {
        while (f->nbOcc == 0) {
            pthread_cond_wait(&(f->vLect), &(f->m));
        }
    } else {
        clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec += (timeout/1000);
        t.tv_nsec = 0;

        while (f->nbOcc == 0 && ret == 0) {
            ret=pthread_cond_timedwait(&(f->vLect), &(f->m), &t);
            //printf("ret %d \n", ret); // 110 pour timeout echu sur Ubuntu 14.04 LTS
        }
        // if (ret == ETIMEDOUT)// non present sur la version Ubuntu
        if (ret != 0) { // timeout : on n ecrit rien dans la file
            pthread_mutex_unlock(&(f->m));
            return 0;
        }
    } // fin cas non bloquant

    memcpy(buf, f->laFile[f->iLect],f->tailleElt);
    f->nbDispo++;
    f->nbOcc--;
    f->iLect=(f->iLect + 1)%f->tailleMax;
    pthread_cond_broadcast(&(f->vEcr));
    pthread_mutex_unlock(&(f->m));

    return 1;

}

// Libere la memoire allouee pour la file. A appeller une seule fois.
int detruireFile(typ_FILE_ATT *f)
{
// f : l adresse de la file
// renvoie 1 : OK,  -1 : erreur (file non initialisee)
    int i;
    if(f->laFile != NULL) {
        for (i=0; i<f->tailleMax; i++) {
            if (f->laFile[i] != NULL) {
                free(f->laFile[i]);
                f->laFile[i]=NULL;
            }
        }

        free(f->laFile);
        f->laFile = NULL;
        return 1;
    } else
        return -1;

}

