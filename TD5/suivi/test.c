#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *my_thread_process (void * arg)
{
  int i;
   int * xx = (int*)arg;
  for (i = 0 ; i < 5 ; i++) {
    printf ("Thread %d: %d\n", *xx, i);
    sleep(1);
  }
  pthread_exit(0);
}

int main()
{
  pthread_t th1, th2;
  void *ret;
  int fd=3;

  if (pthread_create (&th1, NULL, my_thread_process, &fd) < 0) {
    fprintf (stderr, "pthread_create error for thread 1\n");
    exit (1);
  }

  if (pthread_create (&th2, NULL, my_thread_process, &fd) < 0) {
    fprintf (stderr, "pthread_create error for thread 2\n");
    exit (1);
  }

  (void)pthread_join (th1, &ret);
  (void)pthread_join (th2, &ret);

  return(0);
}
