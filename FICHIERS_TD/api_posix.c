/* ex_thread1.c */
/* Creation de 2 threads qui affichent leurs parametres */
/* Creation d un thread qui retourne une valeur */
/* gcc -Wall -lpthread -lposix4 -o thread1 ex_thread1.c */

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *foncThread1(void *arg){
  int val = (int)arg;
  printf("argument thread 1 :%d:\n", val);
  pthread_exit(NULL);
}

void *foncThread2(void *arg){
  char *ch=(char *)arg;
  printf("argument thread 2 :%s:\n", ch);
  pthread_exit(NULL);
}

void *foncThread3(void *arg){
  int i=10; ;
  pthread_exit((void *)i);
}

int main(void){

  pthread_t idThread1, idThread2,  idThread3; /* identifiant des threads */
  int val=5;
  char *chaine="bonjour";
  void *retour;

  /* creation thread 1*/
  if (pthread_create(&idThread1, NULL, foncThread1, (void*)val)!=0){
    printf("erreur creation thread1\n");
    exit(1);
  }

  /* creation thread 2*/
  if (pthread_create(&idThread2, NULL, foncThread2, (void*)chaine)!=0){
    printf("erreur creation thread2\n");
    exit(1);
  }

  /* creation thread 3*/
  if (pthread_create(&idThread3, NULL, foncThread3, NULL)!=0){
    printf("erreur creation thread3\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThread1, NULL);
  pthread_join(idThread2, NULL);
  pthread_join(idThread3, &retour);

  /* affichage de la valeur retournee par le thread 3*/
  printf("valeur de retour thread 3 :%d:\n", (int)retour);

  return 0;
}




/* ex_thread2.c */
/* Execution concurrente de 2 threads */
/* gcc -Wall -lpthread -lposix4 -o thread2 ex_thread2.c */

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int _cpt;

void *foncThread1(void *arg){
  unsigned int savcpt;
  while (_cpt<5){
    savcpt=_cpt;
    printf("Thread1: _cpt=%u - ", _cpt);
    _cpt++;
    printf(" %u+1=%u\n",savcpt, _cpt);
  }
}

void *foncThread2(void *arg){
  unsigned int savcpt;
  while (_cpt<5){
    savcpt=_cpt;
    printf("Thread2: _cpt=%u - ", _cpt);
    _cpt++;
    printf(" %u+1=%u\n",savcpt, _cpt);
  }
}

int main(void){

  pthread_t idThread1, idThread2; /* identifiant des threads */

  _cpt=0;

  /* creation thread 1*/
  if (pthread_create(&idThread1, NULL, foncThread1, NULL)!=0){
    printf("erreur creation thread1\n");
    exit(1);
  }

  /* creation thread 2*/
  if (pthread_create(&idThread2, NULL, foncThread2, NULL)!=0){
    printf("erreur creation thread2\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThread1, NULL);
  pthread_join(idThread2, NULL);

  printf("thread main :%u:\n", _cpt);

  return 0;
}



/* ex_thread3.c */
/* Synchronisation de 2 threads */
/* gcc -Wall -lpthread -lposix4 -o thread3 ex_thread3.c */

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int _cpt;
pthread_mutex_t _m = PTHREAD_MUTEX_INITIALIZER;

void *foncThread1(void *arg){
  unsigned int savcpt;
  while (_cpt<5){
    pthread_mutex_lock(&_m);
    savcpt=_cpt;
    printf("Thread1: _cpt=%u - ", _cpt);
    _cpt++;
    printf(" %u+1=%u\n",savcpt, _cpt);
    pthread_mutex_unlock(&_m);
  }
}

void *foncThread2(void *arg){
  unsigned int savcpt;
  while (_cpt<5){
    pthread_mutex_lock(&_m);
    savcpt=_cpt;
    printf("Thread2: _cpt=%u - ", _cpt);
    _cpt++;
    printf(" %u+1=%u\n",savcpt, _cpt);
    pthread_mutex_unlock(&_m);
  }
}

int main(void){

  pthread_t idThread1, idThread2; /* identifiant des threads */

  _cpt=0;

  /* creation thread 1*/
  if (pthread_create(&idThread1, NULL, foncThread1, NULL)!=0){
    printf("erreur creation thread1\n");
    exit(1);
  }

  /* creation thread 2*/
  if (pthread_create(&idThread2, NULL, foncThread2, NULL)!=0){
    printf("erreur creation thread2\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThread1, NULL);
  pthread_join(idThread2, NULL);

  printf("thread main :%u:\n", _cpt);

  return 0;
}



/* ex_thread5.c */
/* Variable conditionnelle: gestion d'alarme (d' apres Ch. Blaess)*/
/* gcc -Wall -lpthread -lposix4 -o thread5 ex_thread5.c */

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t _condAlarm = PTHREAD_COND_INITIALIZER; /* declaration de la variable conditionnelle */
pthread_mutex_t _m = PTHREAD_MUTEX_INITIALIZER;  /* declaration du mutex */
int alerte = 0;

void *temperature(void *arg){
  int t = 20, s;
  while(1){
    t+=(rand_r(&s)%9) - 5;
    printf("temperature = %d\n", t);
    if((t<16)||(t>24)){
      pthread_mutex_lock(&_m);
      alerte = 1;
      pthread_cond_signal(&_condAlarm);
      pthread_mutex_unlock(&_m);
    }
    else
      alerte = 0;
    sleep(1);
  } /* fin while*/
}

void *alarme(void *arg){
  while(1){
    pthread_mutex_lock(&_m);
    while (!alerte)
        pthread_cond_wait(&_condAlarm, &_m);
    pthread_mutex_unlock(&_m);
    printf("alarme\n");
    sleep(1);
  } /* fin while*/
}

int main(void){

  pthread_t idThreadT, idThreadA; /* identifiant des threads */


  /* creation thread 1*/
  if (pthread_create(&idThreadT, NULL, temperature, NULL)!=0){
    printf("erreur creation thread1\n");
    exit(1);
  }

  /* creation thread 2*/
  if (pthread_create(&idThreadA, NULL, alarme, NULL)!=0){
    printf("erreur creation thread2\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThreadT, NULL);
  pthread_join(idThreadA, NULL);

  printf("fin thread main\n");

  return 0;
}


// normalement thread6 ?
/* ex_thread7.c */
/* threads et fichiers: concurrence */
/* gcc -Wall -lpthread -lposix4 -o thread7 ex_thread7.c */

#define _REENTRANT

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int _fd;

void *fonction1(void *arg){
  int i;
  char buf[50];

 i=0;
  while((i<50)&&(read(_fd, &buf[i], 1)>0)){
    i++;
  }
  buf[i]='\0';
  printf("thread1 :%s:", buf);

  pthread_exit(NULL);

}

void *fonction2(void *arg){
  int i;
  char buf[50];

  i=0;
  while((i<50)&&(read(_fd, &buf[i], 1)>0)){
    i++;
  }
  buf[i]='\0';
  printf("thread2 :%s:", buf);


  pthread_exit(NULL);
}

int main(void){

  pthread_t idThread1, idThread2; /* identifiant des threads */

  if ((_fd=open("toto.txt", O_RDONLY))==-1){
    perror("main ouverture fichier:");
    pthread_exit(NULL);
  }

  /* creation thread 1*/
  if (pthread_create(&idThread1, NULL, fonction1, NULL)!=0){
    printf("erreur creation thread1\n");
    exit(1);
  }

  /* creation thread 2*/
  if (pthread_create(&idThread2, NULL, fonction2, NULL)!=0){
    printf("erreur creation thread2\n");
    exit(1);
  }


  /* ex_thread7.c */
/* threads et fichiers: concurrence */
/* gcc -Wall -lpthread -lposix4 -o thread7 ex_thread7.c */

#define _REENTRANT

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int _fd;

void *fonction1(void *arg){
  int i;
  char buf[50];

 i=0;
  while((i<50)&&(pread(_fd, &buf[i], 1, i)>0)){
    i++;
  }
  buf[i]='\0';
  printf("thread1 :%s:", buf);

  pthread_exit(NULL);

}

void *fonction2(void *arg){
  int i;
  char buf[50];

  i=0;
  while((i<50)&&(pread(_fd, &buf[i], 1, i)>0)){
    i++;
  }
  buf[i]='\0';
  printf("thread2 :%s:", buf);


  pthread_exit(NULL);
}

int main(void){

  pthread_t idThread1, idThread2; /* identifiant des threads */

  if ((_fd=open("toto.txt", O_RDONLY))==-1){
    perror("main ouverture fichier:");
    pthread_exit(NULL);
  }

  /* creation thread 1*/
  if (pthread_create(&idThread1, NULL, fonction1, NULL)!=0){
    printf("erreur creation thread1\n");
    exit(1);
  }

  /* creation thread 2*/
  if (pthread_create(&idThread2, NULL, fonction2, NULL)!=0){
    printf("erreur creation thread2\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThread1, NULL);
  pthread_join(idThread2, NULL);

  close(_fd);

  printf("fin thread main\n");

  return 0;
}


/* ex_thread8.c */
/* Cohorte : ouverture barriere d un parking */
/* cf support cours Programmation Concurrente */
/* gcc -Wall -lpthread -o thread8 ex_thread8.c */

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

void detectionEntree(); /* fonction bloquante tant qu aucune voiture ne se presente */
void detectionSortie(); /* fonction bloquante tant qu aucune voiture ne se presente */

void ouvertureEntree(); /* fonction actionnant l ouverture d une barriere */
void ouvertureSortie(); /* fonction actionnant l ouverture d une barriere */

sem_t _nbLibr; /* declaration du semaphore nb places libres  */


void *entree(void *arg){

  while(1){
    /* Attente detection voiture */
    detectionEntree();

    /*P(nbLibres) : bloquant si pas de place */
    if (sem_wait(&_nbLibr)==-1){
      perror("Entree:erreur wait:");
      pthread_exit(NULL);
    }
    /* ouverture de la barriere */
    ouvertureEntree();

  } /*fin while */

}

void *sortie(void *arg){

  while(1){
   /* Attente detection voiture */
    detectionSortie();

    /*V(nbLibres) : incremente le nombre de places libres */
    if (sem_post(&_nbLibr)==-1){
      perror("Sortie:erreur post:");
      pthread_exit(NULL);
    }
    /* ouverture de la barriere */
    ouvertureSortie();

  }/*fin while */
}

int main(void){

  pthread_t idThreadE, idThreadS; /* identifiant des threads */

  /* initialisation semaphore au nombre de places du parking */
  if (sem_init(&_nbLibr, 0, 50)==-1){
    perror("main:erreur init nbLibr:");
    exit(1);
  }

  /* creation thread entree */
  if (pthread_create(&idThreadE, NULL, entree, NULL)!=0){
    printf("erreur creation thread entrees\n");
    exit(1);
  }

  /* creation thread sortie */
  if (pthread_create(&idThreadS, NULL, sortie, NULL)!=0){
    printf("erreur creation thread sorties\n");
    exit(1);
  }

  /* attente de la terminaison des threads */
  pthread_join(idThreadE, NULL);
  pthread_join(idThreadS, NULL);

/* suppression du semaphores */
  sem_destroy(&_nbLibr);

  return 0;
}


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
int initFile(typ_FILE_ATT *f, int tailleElt, int tailleMax)  {
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


for (i=0;i<tailleMax;i++) {
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
int ecriFile(typ_FILE_ATT *f, char* buf, int timeout) {
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
    	}
    	else {
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
int lireFile(typ_FILE_ATT *f, char *buf, int timeout) {
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
    	}
    	else {
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
int detruireFile(typ_FILE_ATT *f) {
// f : l adresse de la file
// renvoie 1 : OK,  -1 : erreur (file non initialisee)
	int i;
	if(f->laFile != NULL) {
		for (i=0;i<f->tailleMax;i++) {
			if (f->laFile[i] != NULL) {
				free(f->laFile[i]);
				f->laFile[i]=NULL;
			}
		}

		free(f->laFile);
		f->laFile = NULL;
		return 1;
	}
	else
		return -1;

}








  /* attente de la terminaison des threads */
  pthread_join(idThread1, NULL);
  pthread_join(idThread2, NULL);

  close(_fd);

  printf("fin thread main\n");

  return 0;
}



