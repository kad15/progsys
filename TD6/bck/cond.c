#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_cond_t _condAlarm = PTHREAD_COND_INITIALIZER; /* declaration de la variable conditionnelle */
pthread_mutex_t _m = PTHREAD_MUTEX_INITIALIZER; /* declaration du mutex */
int alerte = 0;
void *temperature(void *arg)
{   arg=arg;
    int t = 20;
    unsigned int s;
    while(1)
    {
        t+=(rand_r(&s)%9) - 5;
        printf("temperature = %d\n", t);
        if((t<16)||(t>24))
        {
            pthread_mutex_lock(&_m);
            alerte = 1;
            pthread_cond_signal(&_condAlarm); // Signal
            pthread_mutex_unlock(&_m);
        }
        else
            alerte = 0;
        sleep(1);
    } /* fin while*/
}
void *alarme(void *arg)
{ arg=arg;
    while(1)
    {
        pthread_mutex_lock(&_m);
        while (!alerte)
            pthread_cond_wait(&_condAlarm, &_m); // Attente
        pthread_mutex_unlock(&_m);
        printf("alarme\n");
        sleep(1);
    } /* fin while*/
}
int main(void)
{
    pthread_t idThreadT, idThreadA; /* identifiant des threads */
    /* creation thread 1*/
    if (pthread_create(&idThreadT, NULL, temperature, NULL)!=0)
    {
        printf("erreur creation thread1\n");
        exit(1);
    }
    /* creation thread 2*/
    if (pthread_create(&idThreadA, NULL, alarme, NULL)!=0)
    {
        printf("erreur creation thread2\n");
        exit(1);
    }

    /* attente de la terminaison des threads */
    pthread_join(idThreadT, NULL);
    pthread_join(idThreadA, NULL);
    printf("fin thread main\n");
    return 0;
}
